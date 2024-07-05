#include "instrucciones.hpp"

Intrucciones::Intrucciones(Contexto &contx) : contx(contx)
{
    camara = Camara_Ortografica(glm::vec3(0.0f, 0.0f, 0.0f), contx.ancho_ventana, contx.alto_ventana);

    stbi_set_flip_vertically_on_load(true);
    cargar_textura("../assets/textures/instructions.png", instrucciones_textura);

    creditos_shader = Shader("button.vs", "button.fs");
    escena_guardada_ctx();
}

void Intrucciones::escena_guardada_ctx()
{
    contx.escenas.push_back(this);
}

void Intrucciones::apertura_de_escena() 
{
    glfwSetInputMode(contx.ventana, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    reloj.actualizacion_de_escena();
    start_time = reloj.tiempo_actual;
}

void Intrucciones::cierre_de_escena() { return; }

void Intrucciones::actualizacion_de_escena()
{
    reloj.actualizacion_de_escena();

    if (reloj.tiempo_actual - start_time >= 6.0f)
    {
        contx.cargar_escena_id(2);
    }

    creditos_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, instrucciones_textura);
    creditos_shader.set_int("texture0", 0);
    forma.render(creditos_shader, camara);
}

void Intrucciones::limpieza_de_escena()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Intrucciones::proceso_de_entrada(){}

void Intrucciones::llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) 
{
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    xpos -= contx.ancho_ventana / 2;
    ypos = contx.alto_ventana - ypos;
    ypos -= contx.alto_ventana / 2;
}

void Intrucciones::click_izquierdo(GLFWwindow* ventana, int button, int action, int mods) 
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

void Intrucciones::scroll(GLFWwindow* ventana, double xoffset, double yoffset) {}

void Intrucciones::llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto)
{
    glViewport(0, 0, ancho, alto);
    camara.ancho = ancho;
    camara.alto = alto;

    //redimensiona la intruccion
    float backgroundar = 800.0f / 600.0f;
    float factor_escalado;

    factor_escalado = contx.alto_ventana / 600.0f;

    forma.transform.scale.x = 800.0f * factor_escalado;
    forma.transform.scale.y = 600.0f * factor_escalado;
}