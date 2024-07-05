#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "controlador_camara.hpp"

class Camara_Ortografica : public controlador_camara {
    public:
        Camara_Ortografica(glm::vec3 posicion = glm::vec3(0.0f, 0.0f, 0.0f), float ancho_ventana = 800, float alto_ventana = 600) : front(glm::vec3(0.0f, 0.0f, -1.0f))
        {
            posicion = posicion;
            ancho = ancho_ventana;
            alto = alto_ventana;

            velocidad_movimiento = 2.5f;
            sensibilidad = 0.1f;
            world_up = glm::vec3(0.0f, 1.0f, 0.0f);

            update_camera_vectors();
        }

        glm::mat4 get_view_matrix() const
        {
            return glm::lookAt(posicion, posicion + front, arriba);
        }

        glm::mat4 get_projection_matrix() const
        {

            return glm::ortho(-ancho/2.0f, ancho/2.0f, -alto/2.0f, alto/2.0f, -1.0f, 1.0f);
        }

    public:
        glm::vec3 posicion;
        glm::vec3 front;
        glm::vec3 arriba;
        glm::vec3 derec;
        glm::vec3 world_up;
        
        float yaw;
        float pitch;
        
        float velocidad_movimiento;
        float sensibilidad;
        float cdv;

        float ancho;
        float alto;

        bool fps;
        
    private:
        void update_camera_vectors()
        {
            derec = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            arriba = glm::normalize(glm::cross(derec, front));
        }
};