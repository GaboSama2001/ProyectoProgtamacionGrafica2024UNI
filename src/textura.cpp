#include "textura.hpp"
#include <stb_image.h>

void cargar_textura(const char *file, unsigned int &textura)
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //second argument was GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //second argument was GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << file << std::endl;
        stbi_image_free(data);
    }
}