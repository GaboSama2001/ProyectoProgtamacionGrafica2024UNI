#pragma once

#include "shader.h"
#include "formas.hpp"
#include "controlador_camara.hpp"

class Boton {
    public:
        rectangulo forma;
        unsigned int textura;

        bool hover = false;

    public:
        Boton(float posicionx = 0, float posiciony = 0, float ancho = 150, float alto = 100) : boton_posicionx(posicionx), boton_posiciony(posiciony), boton_ancho(ancho), boton_alto(alto)
        {
            forma.transform.posicion.x = boton_posicionx;
            forma.transform.posicion.y = boton_posiciony;
            forma.transform.scale.x = boton_ancho;
            forma.transform.scale.y = boton_alto;
        };

        void render(Shader shader, const controlador_camara &camara) 
        {
            forma.render(shader, camara);
        };

        bool is_hovered(double xpos, double ypos) 
        {
            if (xpos >= forma.transform.posicion.x - forma.transform.scale.x / 2 && 
                    xpos <= forma.transform.posicion.x + forma.transform.scale.x / 2 && 
                    ypos >= forma.transform.posicion.y - forma.transform.scale.y / 2 && 
                    ypos <= forma.transform.posicion.y + forma.transform.scale.y / 2) {hover = true;}
            else hover = false;

            return hover;
        }

    private:
        float boton_posicionx;
        float boton_posiciony;
        float boton_ancho;
        float boton_alto;
};