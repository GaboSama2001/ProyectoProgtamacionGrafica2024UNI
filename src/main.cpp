#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>//hello

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "contexto.hpp"
#include "escena_juego.hpp"
#include "menu_escena.hpp"
#include "creditos.hpp"
#include "constructor.hpp"
#include "instrucciones.hpp"

int main()
{
    Contexto contx;
    glfwSwapInterval(1); //vsync

    constructor splashscreen(contx); // idx 0
    Intrucciones instructions(contx); //idx 1
    Menu_Escena menu(contx); //idx 2
    Escena_Juego scene(contx); // idx 3
    Creditos credits(contx); //idx 4

    contx.cargar_escena(contx.escenas[0]);
    contx.run();
    glfwTerminate();
    return 0;
}