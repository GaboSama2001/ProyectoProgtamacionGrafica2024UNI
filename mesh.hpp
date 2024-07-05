#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // posicion
    glm::vec3 posicion;
    // normal
    glm::vec3 normal;
    // texCoordenadas
    glm::vec2 tex_coords;
    // tangente
    glm::vec3 tangent;
    // bitangente
    glm::vec3 bitangent;
	//bone indexes which will influence this vertex
	int m_bone_ids[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textura;
    unsigned int vao;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textura)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textura = textura;

        setup_mesh();
    }

    // render the mesh
    void draw(Shader &shader)
    {
        // bind appropriate textura
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < textura.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper textura unit before binding
            // retrieve textura number (the N in diffuse_textureN)
            string number;
            string name = textura[i].type;
            if(name == "material.texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "material.texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if(name == "material.texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
             else if(name == "material.texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct textura unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the textura
            glBindTexture(GL_TEXTURE_2D, textura[i].id);
        }
        
        // draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data 
    unsigned int vbo, ebo;

    // initializes all the buffer objects/arrays
    void setup_mesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex textura coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
		// ids
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_bone_ids));

		// weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));
        glBindVertexArray(0);
    }
};
#endif