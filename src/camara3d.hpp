#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "controlador_camara.hpp"

enum Movimiento_Camara3D  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SENSITIVITY =  0.1f;
const float FOV         =  45.0f;
const glm::vec3 UP      =  glm::vec3(0.0f, 1.0f, 0.0f);

class Camara3D : public controlador_camara {
    public:
        Camara3D(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float ancho_ventana = 800, float alto_ventana = 600, float speed = 2.5f, bool is_fps = false) : front(glm::vec3(0.0f, 0.0f, -1.0f)), world_up(UP), sensibilidad(SENSITIVITY), cdv(FOV), pich(PITCH), yau(YAW)
        {
            posicion = pos;
            posicion_inicial = pos;
            ancho = ancho_ventana;
            alto = alto_ventana;
            fps = is_fps;
            velocidad_movimiento = speed;
            update_camera_vectors();
        }

        glm::mat4 get_view_matrix() const
        {
            return glm::lookAt(posicion, posicion + front, arriba);
        }

        glm::mat4 get_projection_matrix() const
        {
            return glm::perspective(glm::radians(cdv), ancho / alto, 0.1f, 100.0f);
        }

        void process_keyboard(Movimiento_Camara3D direccion, float tiempo_final)
        {
            float velocidad = velocidad_movimiento * tiempo_final;
            if (direccion == FORWARD)
                posicion += front * velocidad;
            if (direccion == BACKWARD)
                posicion -= front * velocidad;
            if (direccion == LEFT)
                posicion -= derec * velocidad;
            if (direccion == RIGHT)
                posicion += derec * velocidad;
            if (fps)
                posicion.y = posicion_inicial.y;
        }

        void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= sensibilidad;
            yoffset *= sensibilidad;

            yau   += xoffset;
            pich += yoffset;

            if (constrainPitch)
            {
                if (pich > 89.0f)
                    pich = 89.0f;
                if (pich < -89.0f)
                    pich = -89.0f;
            }

            update_camera_vectors();
        }

        void process_mouse_scroll(float yoffset)
        {
            cdv -= (float)yoffset;
            if (cdv < 1.0f)
                cdv = 1.0f;
            if (cdv > 45.0f)
                cdv = 45.0f;
        }

    public:
        glm::vec3 posicion;
        glm::vec3 posicion_inicial;
        glm::vec3 front;
        glm::vec3 arriba;
        glm::vec3 derec;
        glm::vec3 world_up;
        
        float yau;
        float pich;
        
        float velocidad_movimiento;
        float sensibilidad;
        float cdv;

        float ancho;
        float alto;

        bool fps;

    private:
        void update_camera_vectors()
        {
            // calcular el nuevo vector frontal
            glm::vec3 new_front;
            new_front.x = cos(glm::radians(yau)) * cos(glm::radians(pich));
            new_front.y = sin(glm::radians(pich));
            new_front.z = sin(glm::radians(yau)) * cos(glm::radians(pich));
            front = glm::normalize(new_front);
            // también vuelve a calcular el vector Derecha y Arriba
            derec = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            arriba = glm::normalize(glm::cross(derec, front));
        }
};