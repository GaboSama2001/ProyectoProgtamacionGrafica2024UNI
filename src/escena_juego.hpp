#pragma once

#include "shader.h"
#include "formas.hpp"
#include "controlador_escena.hpp"
#include "contexto.hpp"
#include "boton.hpp"
#include "cielo.hpp"
#include "modelo.hpp"
#include "reloj.hpp"

#include "mapa.hpp"
#include "jugador.hpp"
#include "enemigo.hpp"
#include "textura.hpp"

class Escena_Juego : public Controlador_escena {
    public:
        Escena_Juego(Contexto &contx);
        void escena_guardada_ctx() override;
        void apertura_de_escena() override;
        void cierre_de_escena() override;
        void actualizacion_de_escena() override;
        void limpieza_de_escena() override;
        void proceso_de_entrada() override;
        void llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) override;
        void click_izquierdo(GLFWwindow* ventana, int button, int action, int mods) override;
        void scroll(GLFWwindow* ventana, double xoffset, double yoffset) override;
        void llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto) override;

        void inicio_framebuffer();
        void configuraciones_shaders();
        void susto();
        void final_condicion();


    public:
        Contexto &contx;
        bool susto_activacion = false;
    private:
        float lastX = contx.ancho_ventana / 2.0f;
        float lastY = contx.alto_ventana / 2.0f;
        bool mouse_firstview = true;
    
        Jugador *jugador = nullptr;
        Enemigo *enemy = nullptr;

        Mapa mapa;
        Shader escena_shader;
        Shader pantalla_shader;
        Shader luz_shader;
        Shader mapa_shader;
        Shader suelo_shader;
        
        Reloj reloj;
        glm::vec3 light_pos;

        Shader modelShader;
    
        float scream_start_time;
        ma_sound sonido_grito;
        ma_sound sonido_ambiente;

        unsigned int mascara_id;
    
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int rbo;
        unsigned int quadvao;
        unsigned int quadvbo;
        float quadVertices[24] = { 
            // posiciones   // coordenadas de texto
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f, 1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };
};