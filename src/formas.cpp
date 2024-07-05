#include "formas.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Cubo::render(Shader shader, const controlador_camara& camara)
{
    shader.use();
    if (textura_difusa) {
        shader.set_int("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textura_difusa);
    }

    if (textura_especular) {
        shader.set_int("material.specular", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textura_especular);
    }

    glm::mat4 modelo = transform.get_model_matrix();
    glm::mat4 proyeccion = camara.get_projection_matrix();
    glm::mat4 vista = camara.get_view_matrix();
    glm::mat3 norm = glm::transpose(glm::inverse(modelo));

    shader.set_mat4("model", modelo);
    shader.set_mat4("projection", proyeccion);
    shader.set_mat4("view", vista);
    shader.set_mat3("normal", norm);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

//3D PRIMITIVES
Cubo::Cubo()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}



void Cubo::add_texture(const char *file, unsigned int &textura)
{
    glGenTextures(1, &textura);
    
    int ancho, alto, nrComponents;
    unsigned char *data = stbi_load(file, &ancho, &alto, &nrComponents, 0);
    if (data)
    {
        GLenum formato;
        if (nrComponents == 1)
            formato = GL_RED;
        else if (nrComponents == 3)
            formato = GL_RGB;
        else if (nrComponents == 4)
            formato = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textura);
        glTexImage2D(GL_TEXTURE_2D, 0, formato, ancho, alto, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "La textura no se pudo cargar en la ruta : " << file << std::endl;
        stbi_image_free(data);
    }
}