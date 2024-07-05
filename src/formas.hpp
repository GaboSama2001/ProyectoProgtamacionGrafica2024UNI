#ifndef FORMA_HPP
#define FORMA_HPP

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "transformaciones.hpp"
#include "controlador_camara.hpp"

//2D PRIMITIVES
class triangulos{
    public:
        triangulos() {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, triangulo_vertices.size() * sizeof(float), triangulo_vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        void render(Shader shader, const controlador_camara &camara)
        {
            shader.use();
            glm::mat4 math = transform.get_model_matrix();
            glm::mat4 proyeccion = camara.get_projection_matrix();
            glm::mat4 vista = camara.get_view_matrix();

            shader.set_mat4("modelo", math);
            shader.set_mat4("proyeccion", proyeccion);
            shader.set_mat4("vista", vista);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

    public:
        Transformaciones transform;

    private:
        unsigned int vao;
        unsigned int vbo;

        std::vector<float> triangulo_vertices = {
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
};

class rectangulo {
    public:
        rectangulo() {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &ebo);
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(rectangulo_vertices), rectangulo_vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        void render(Shader shader, const controlador_camara &camara)
        {
            shader.use();
            glm::mat4 math = transform.get_model_matrix();
            glm::mat4 proyeccion = camara.get_projection_matrix();
            glm::mat4 vista = camara.get_view_matrix();

            shader.set_mat4("model", math);
            shader.set_mat4("projection", proyeccion);
            shader.set_mat4("view", vista);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    public:
        Transformaciones transform;
        unsigned int textura;

    private:
        unsigned int ebo;
        unsigned int vao;
        unsigned int vbo;

        float rectangulo_vertices[20] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// fondo izquierda
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,// fondo derecha
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,// arriba derecha
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,// arriba izquierda
        };

        unsigned int indices[6] = {
            0, 1, 2,   
            0, 2, 3
        };
};

//3D PRIMITIVES
class Cubo {
    public:
        Cubo();
        void render(Shader shader, const controlador_camara &camara);
        void add_texture(const char *file, unsigned int &textura);

    public:
        Transformaciones transform;
        unsigned int textura_difusa;
        unsigned int textura_especular;
    private:
        unsigned int vao;
        unsigned int vbo;
        unsigned int textura;
        //pos 3 //textura coords 2 // normal 3
        std::vector<float> vertices
        {
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f,      1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f,      1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f, 0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f, 0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,     1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,

            -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
            0.5f, 0.5f, -0.5f,     1.0f, 1.0f,    0.0f, 1.0f,  0.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            0.5f, 0.5f, 0.5f,      1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            -0.5f, 0.5f, 0.5f,     0.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f,  0.0f
        };
};

#endif