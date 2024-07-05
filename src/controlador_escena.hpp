#pragma once

#include <glad/glad.h>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Controlador_escena {
    public:
        virtual ~Controlador_escena(){}
        virtual void escena_guardada_ctx() = 0;
        virtual void apertura_de_escena() = 0;
        virtual void cierre_de_escena() = 0;
        virtual void actualizacion_de_escena() = 0;
        virtual void limpieza_de_escena() = 0;
        virtual void proceso_de_entrada() = 0;
        virtual void llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) = 0;
        virtual void click_izquierdo(GLFWwindow* ventana, int button, int action, int mods) = 0;
        virtual void scroll(GLFWwindow* ventana, double xoffset, double yoffset) = 0;
        virtual void llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto) = 0;
};