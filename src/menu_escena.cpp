#include "menu_escena.hpp"

Menu_Escena::Menu_Escena(Contexto &contx) : contx(contx)
{
    camara = Camara_Ortografica(glm::vec3(0.0f, 0.0f, 0.0f), contx.ancho_ventana, contx.alto_ventana);

    boton_inicio = Boton(-400.0f, 100.0f, 200.0f, 80.0f);
    boton_salida = Boton(-400.0f,  0.0f, 200.0f, 80.0f);
    boton_creditos = Boton(-400.0f,  -100.0f, 200.0f, 80.0f);

    stbi_set_flip_vertically_on_load(true);
    cargar_textura("../assets/textures/menu_bg.png", bg.textura);
    cargar_textura("../assets/textures/play_button.png", boton_inicio.textura);
    cargar_textura("../assets/textures/quit_button.png", boton_salida.textura);
    cargar_textura("../assets/textures/credits_button.png", boton_creditos.textura);

    boton_shader = Shader("button.vs", "button.fs");
    suelo_shader = Shader("bg_texture.vs", "bg_texture.fs");
    
    escena_guardada_ctx();
}

void Menu_Escena::escena_guardada_ctx()
{
    contx.escenas.push_back(this);
}

void Menu_Escena::apertura_de_escena() 
{
    glDisable(GL_DEPTH_TEST);
    glfwSetInputMode(contx.ventana, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Menu_Escena::cierre_de_escena() { return; }

void Menu_Escena::actualizacion_de_escena()
{
    reloj.actualizacion_de_escena();
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, bg.textura);
    bg.render(suelo_shader, camara);

    boton_shader.use();
    boton_shader.set_int("texture0", 0);
    
    glBindTexture(GL_TEXTURE_2D, boton_inicio.textura);
    boton_shader.set_bool("hovered", boton_inicio.hover);
    boton_inicio.render(boton_shader, camara);

    glBindTexture(GL_TEXTURE_2D, boton_salida.textura);
    boton_shader.set_bool("hovered", boton_salida.hover);
    boton_salida.render(boton_shader, camara);

    glBindTexture(GL_TEXTURE_2D, boton_creditos.textura);
    boton_shader.set_bool("hovered", boton_creditos.hover);
    boton_creditos.render(boton_shader, camara);
}

void Menu_Escena::limpieza_de_escena()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Menu_Escena::proceso_de_entrada() {}

void Menu_Escena::llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;

    boton_inicio.is_hovered(xpos, ypos);
    boton_salida.is_hovered(xpos, ypos);
    boton_creditos.is_hovered(xpos, ypos);
}

void Menu_Escena::click_izquierdo(GLFWwindow* ventana, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        if(boton_inicio.hover) {
            contx.cargar_escena_id(3);
        }
        if(boton_salida.hover) {
            glfwSetWindowShouldClose(contx.ventana, true);
        }
        if(boton_creditos.hover) {
            contx.cargar_escena_id(4);
        }
    }
}

void Menu_Escena::scroll(GLFWwindow* ventana, double xoffset, double yoffset) {}

void Menu_Escena::llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto)
{
    glViewport(0, 0, ancho, alto);
    camara.ancho = ancho;
    camara.alto = alto;

    float backgroundar = 1665.0f / 1369.0f;
    float factor_escalado;

    factor_escalado = contx.ancho_ventana / 1665.0f;

    bg.transform.scale.x = 1665.0f * factor_escalado;
    bg.transform.scale.y = 1369.0f * factor_escalado;
}