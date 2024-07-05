#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sonido.hpp"
#include "controlador_escena.hpp"

class Contexto
{
    public://parametros y nombre de la ventana
        Contexto(float ancho = 1280, float alto = 720, const char *nombre = "Juego de miedo") {
            ancho_ventana = ancho;
            alto_ventana = alto;
            nombre_ventana = nombre;
            ventana = creacion_ventana();
            cargar_glad();
            esteblecer_devolucion_llamada();
        };

        void esteblecer_devolucion_llamada()
        {//Buffer de la ventana
            glfwSetWindowUserPointer(ventana, this);
            glfwSetFramebufferSizeCallback(ventana, llamado_tamaño_framebuffer_wrapper);
            glfwSetCursorPosCallback(ventana, mouse_callback_wrapper);
            glfwSetMouseButtonCallback(ventana, left_click_callback_wrapper);
            glfwSetScrollCallback(ventana, scroll_callback_wrapper);  
        }

        void cargar_escena(Controlador_escena *scene)
        {//apertura y cierre
            if (escena_actual != nullptr)
                escena_actual->cierre_de_escena();
            escena_actual = scene;
            escena_actual->apertura_de_escena();
            llamado_tamaño_framebuffer_wrapper(ventana, ancho_ventana, alto_ventana);
        }

        void cargar_escena_id(int id)
        {//carga de escena 
            cargar_escena(escenas[id]);
        }

        void run()
        {//iniciar
            if (escena_actual == nullptr) 
            { 
                std::cout << "Debe asignar una escena real." << std::endl; 
                return;
            }
            while (!glfwWindowShouldClose(ventana))
            {
                escena_actual->limpieza_de_escena();
                escena_actual->proceso_de_entrada();
                escena_actual->actualizacion_de_escena();

                glfwSwapBuffers(ventana);
                glfwPollEvents();
            }
            ma_engine_uninit(&centro_sonido.engine);
        }

        GLFWwindow *ventana;
        float ancho_ventana;
        float alto_ventana;
        float aspect_ratio = ancho_ventana / alto_ventana;
        const char *nombre_ventana; 

        Controlador_escena *escena_actual = nullptr;
        std::vector<Controlador_escena*> escenas;
        Sonido centro_sonido;
    
    private:
        GLFWwindow *creacion_ventana() {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            GLFWwindow* ventana = glfwCreateWindow(ancho_ventana, alto_ventana, nombre_ventana, NULL, NULL);
            if (ventana == NULL)
            {
                std::cout << "Falla al crear GLFW ventana" << std::endl;
                glfwTerminate();
                return NULL;
            }
            glfwMakeContextCurrent(ventana);
            return ventana;
        };

        int cargar_glad() 
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "Falla al inicializar GLAD" << std::endl;
                std::cout << "No se puede crear la App" << std::endl;
                return -1;
            }
            return 0;
        };

        static void llamado_tamaño_framebuffer_wrapper(GLFWwindow* ventana, int ancho, int alto) 
        {
            Contexto* contx = static_cast<Contexto*>(glfwGetWindowUserPointer(ventana));
            contx->ancho_ventana = ancho;
            contx->alto_ventana = alto;
            contx->aspect_ratio = ancho / alto;
            if (contx->escena_actual != nullptr) {
                contx->escena_actual->llamado_tamaño_framebuffer(ventana, ancho, alto);
            }   
        }

        static void mouse_callback_wrapper(GLFWwindow* ventana, double xpos, double ypos)
        {
            Contexto* contx = static_cast<Contexto*>(glfwGetWindowUserPointer(ventana));

            if (contx->escena_actual != nullptr) {
                contx->escena_actual->llamado_mouse(ventana, xpos, ypos);
            }
        }

        static void scroll_callback_wrapper(GLFWwindow* ventana, double xoffset, double yoffset)
        {
            Contexto* contx = static_cast<Contexto*>(glfwGetWindowUserPointer(ventana));

            if (contx->escena_actual != nullptr) {
                contx->escena_actual->scroll(ventana, xoffset, yoffset);
            }
        }

        static void left_click_callback_wrapper(GLFWwindow* ventana, int button, int action, int mods) 
        {
            Contexto* contx = static_cast<Contexto*>(glfwGetWindowUserPointer(ventana));

            if (contx->escena_actual != nullptr) {
                contx->escena_actual->click_izquierdo(ventana, button, action, mods);
            }

        }
};

#endif