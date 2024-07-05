#include "shader.h"
#include "formas.hpp"
#include "camara_ortografica.hpp"
#include "controlador_escena.hpp"
#include "contexto.hpp"
#include "boton.hpp"
#include "textura.hpp"
#include "stb_image.h"

class Creditos : public Controlador_escena {
    public:
        Creditos(Contexto &contx);
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

    public:
        Contexto &contx;
    
    private:
        float TiempoDelta = 0.0f;	// time between actual frame and last frame
        float FrameFinal = 0.0f;

        Camara_Ortografica camara;
        Shader creditos_shader;
        rectangulo forma;
        unsigned int textura_creditos;
};