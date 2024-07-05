#pragma once

#include <vector>
#include <string>
#include <iostream>

// Incluir GLEW antes de cualquier otra cabecera de OpenGL
#include <stb_image.h>

#include "shader.h"
#include "controlador_camara.hpp"

class Skybox {
public:
    Skybox() {
        // Generación y configuración del VAO y VBO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(float), skybox_vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Rutas a las texturas del cubemap
        std::vector<std::string> caras = {
            "../ProyectoProgtamacionGrafica2024UNI/assets/textures/skybox/right.jpg",
            "../ProyectoProgtamacionGrafica2024UNI/assets/textures/skybox/left.jpg",
            "../ProyectoProgtamacionGrafica2024UNI/assets/textures/skybox/top.jpg",
            "../ProyectoProgtamacionGrafica2024UNI/assets/textures/skybox/bottom.jpg",
            "../ProyectoProgtamacionGrafica2024UNI/assets/textures/skybox/front.jpg",
            "../ProyectoProgtamacionGrafica2024UNI/assets/textures/skybox/back.jpg"
        };

        // Imprimir rutas de archivos para depuración
        for (const auto& ruta : caras) {
            std::cout << "Cargando textura del cubemap desde la ruta: " << ruta << std::endl;
        }

        // Configurar el flipping vertical
        stbi_set_flip_vertically_on_load(false);

        // Carga del cubemap
        textura_cubemap = load_cubemap(caras);

        // Shader del skybox
        skybox_shader = Shader("skybox.vs", "skybox.fs");
        transform.posicion = { 0.0f, 0.0f, 0.0f };
    }

    void render(const controlador_camara& camara) {
        glm::mat4 projection = camara.get_projection_matrix();
        glm::mat4 view = glm::mat4(glm::mat3(camara.get_view_matrix()));
        glDepthFunc(GL_LEQUAL);
        skybox_shader.use();
        skybox_shader.set_mat4("view", view);
        skybox_shader.set_mat4("projection", projection);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textura_cubemap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
    }

public:
    Transformaciones transform;

private:
    unsigned int load_cubemap(std::vector<std::string> caras) {
        unsigned int texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        int ancho, alto, nrChannels;
        for (unsigned int i = 0; i < caras.size(); i++) {
            std::cout << "Intentando cargar textura del cubemap desde la ruta: " << caras[i] << std::endl;
            unsigned char* data = stbi_load(caras[i].c_str(), &ancho, &alto, &nrChannels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else {
                std::cout << "La textura del cubemap no se pudo cargar en la ruta: " << caras[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texture_id;
    }

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int textura_cubemap;
    Shader skybox_shader;
    std::vector<float> skybox_vertices{
        // posiciones
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
};
