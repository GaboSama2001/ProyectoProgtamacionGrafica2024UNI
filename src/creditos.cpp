#include "creditos.hpp"

Creditos::Creditos(Contexto &contx) : contx(contx)
{
    camara = Camara_Ortografica(glm::vec3(0.0f, 0.0f, 0.0f), contx.ancho_ventana, contx.alto_ventana);

    stbi_set_flip_vertically_on_load(true);
    cargar_textura("../assets/textures/credits.png", textura_creditos);
    forma.transform.scale.x = contx.ancho_ventana;
    forma.transform.scale.y = contx.alto_ventana;
    creditos_shader = Shader("button.vs", "button.fs");
    escena_guardada_ctx();
}

void Creditos::escena_guardada_ctx()
{
    contx.escenas.push_back(this);
}

void Creditos::apertura_de_escena() 
{
    glfwSetInputMode(contx.ventana, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Creditos::cierre_de_escena() { return; }

void Creditos::actualizacion_de_escena()
{
    double TiempoActual = glfwGetTime();
    TiempoDelta = TiempoActual - FrameFinal;
    FrameFinal = TiempoActual;

    creditos_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura_creditos);
    creditos_shader.set_int("texture0", 0);
    forma.render(creditos_shader, camara);
}

void Creditos::limpieza_de_escena()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Creditos::proceso_de_entrada()
{
    if (glfwGetKey(contx.ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        contx.cargar_escena_id(2);
}

void Creditos::llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;
}

void Creditos::click_izquierdo(GLFWwindow* ventana, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        contx.cargar_escena_id(2);
    }
}

void Creditos::scroll(GLFWwindow* ventana, double xoffset, double yoffset) {} 

void Creditos::llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto)
{
    glViewport(0, 0, ancho, alto);
    camara.ancho = ancho;
    camara.alto = alto;
    contx.ancho_ventana = ancho;
    contx.alto_ventana = alto;
}