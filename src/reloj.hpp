#pragma once

#include <GLFW/glfw3.h>

class Reloj
{
    public:
        double tiempo_actual;
        float tiempo_final;
        float ultimo_frame;

        Reloj() { actualizacion_de_escena(); };

        void actualizacion_de_escena() {
            tiempo_actual = glfwGetTime();
            tiempo_final = tiempo_actual - ultimo_frame;
            ultimo_frame = tiempo_actual;
        };

        void reset() {
            tiempo_actual = glfwGetTime();
            tiempo_final = tiempo_actual - ultimo_frame;
            ultimo_frame = tiempo_actual;
        }
};