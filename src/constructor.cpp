#include "constructor.hpp"

constructor::constructor(Contexto &contx) : contx(contx)
{
    camara = Camara_Ortografica(glm::vec3(0.0f, 0.0f, 0.0f), contx.ancho_ventana, contx.alto_ventana);

    stbi_set_flip_vertically_on_load(true);
    cargar_textura("../assets/textures/sky.png", textura_creditos);
    forma.transform.scale.x = contx.ancho_ventana;
    forma.transform.scale.y = contx.alto_ventana;
    creditos_shader = Shader("button.vs", "button.fs");
    escena_guardada_ctx();
}

void constructor::escena_guardada_ctx()
{
    contx.escenas.push_back(this);
}

void constructor::apertura_de_escena() 
{
    glfwSetInputMode(contx.ventana, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    start_time = reloj.tiempo_actual;
}

void constructor::cierre_de_escena() { return; }

void constructor::actualizacion_de_escena()
{
    reloj.actualizacion_de_escena();

    if (reloj.tiempo_actual - start_time >= 4.0f)
    {
        contx.cargar_escena_id(1);
    }

    creditos_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura_creditos);
    creditos_shader.set_int("texture0", 0);
    forma.render(creditos_shader, camara);
}

void constructor::limpieza_de_escena()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void constructor::proceso_de_entrada()
{
    if (glfwGetKey(contx.ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(contx.ventana, true);
}

void constructor::llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;
}

void constructor::click_izquierdo(GLFWwindow* ventana, int button, int action, int mods) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;
}

void constructor::scroll(GLFWwindow* ventana, double xoffset, double yoffset) {} 


void constructor::llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto)
{
    glViewport(0, 0, ancho, alto);
    camara.ancho = ancho;
    camara.alto = alto;
}