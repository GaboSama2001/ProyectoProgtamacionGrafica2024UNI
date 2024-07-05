#include "escena_juego.hpp"
#include "miscelaneos.hpp"

Escena_Juego::Escena_Juego(Contexto &contx) : contx(contx)
{
    escena_guardada_ctx();

    mapa.load_map();
    mapa_shader = Shader("basic_light.vs", "map_spotlight.fs");
    suelo_shader = Shader("floor.vs", "floor_spotlight.fs");

    escena_shader = Shader("framebuffer.vs", "framebuffer.fs");
    pantalla_shader = Shader("framebuffer_screen.vs", "framebuffer_screen.fs");

    
    
    jugador = new Jugador(mapa, contx.centro_sonido, contx.ancho_ventana, contx.alto_ventana);
    enemy = new Enemigo(mapa, contx.centro_sonido);

    ma_sound_init_from_file(&contx.centro_sonido.engine, "../assets/sfx/screamer.wav", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &contx.centro_sonido.fence, &sonido_grito);
    ma_sound_init_from_file(&contx.centro_sonido.engine, "../assets/sfx/ambiance.wav", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &contx.centro_sonido.fence, &sonido_ambiente);    
    cargar_textura("../assets/textures/flashlight.png", mascara_id);

    // configuraciones del framebuffer
    glGenVertexArrays(1, &quadvao);
    glGenBuffers(1, &quadvbo);
    glBindVertexArray(quadvao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Escena_Juego::escena_guardada_ctx()
{
    contx.escenas.push_back(this);
}

void Escena_Juego::inicio_framebuffer()
{
    escena_shader.use();
    escena_shader.set_int("texture1", 0);

    pantalla_shader.use();
    pantalla_shader.set_int("screenTexture", 0);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, contx.ancho_ventana, contx.alto_ventana, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, contx.ancho_ventana, contx.alto_ventana);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer no está completo!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Escena_Juego::apertura_de_escena()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(contx.ventana, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    inicio_framebuffer();
    jugador->init();
    enemy->init();
    reloj.reset();
    susto_activacion = false;
    ma_engine_start(&contx.centro_sonido.engine);
    ma_sound_start(&sonido_ambiente);
    ma_sound_set_looping(&sonido_ambiente, true);
}

void Escena_Juego::cierre_de_escena() 
{
    ma_engine_stop(&contx.centro_sonido.engine);
    return;
}

void Escena_Juego::configuraciones_shaders()
{
    mapa_shader.use();

    mapa_shader.set_float("time", reloj.tiempo_actual);
    //spotlight properties
    mapa_shader.set_vec3("light.position", jugador->camara_jugador.posicion);
    mapa_shader.set_vec3("light.direction", jugador->camara_jugador.front);
    mapa_shader.set_float("light.cutOff",   glm::cos(glm::radians(10.0f)));
    mapa_shader.set_float("light.outerCutOff", glm::cos(glm::radians(12.0f)));

    mapa_shader.set_vec3("viewPos", jugador->camara_jugador.posicion);

    // light properties
    if (jugador->torchlight_on)
    {
        mapa_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
        mapa_shader.set_vec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        mapa_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

    }
    else 
    {
        mapa_shader.set_vec3("light.ambient", 0.005f, 0.005f, 0.005f);
        mapa_shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
        mapa_shader.set_vec3("light.specular", 0.0f, 0.0f, 0.0f);
    }

    mapa_shader.set_float("light.constant", 1.0f);
    mapa_shader.set_float("light.linear", 0.22f);
    mapa_shader.set_float("light.quadratic", 0.20f);

    //material properties
    mapa_shader.set_float("material.shininess", 32.0f);

    //torchlight mask
    mapa_shader.set_vec2("viewPort", contx.ancho_ventana, contx.alto_ventana);
    mapa_shader.set_int("light.flashlight", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mascara_id);

    suelo_shader.use();
    suelo_shader.set_vec3("light.position", jugador->camara_jugador.posicion);
    suelo_shader.set_vec3("light.direction", jugador->camara_jugador.front);
    suelo_shader.set_float("light.cutOff",   glm::cos(glm::radians(10.0f)));
    suelo_shader.set_float("light.outerCutOff", glm::cos(glm::radians(12.0f)));

    suelo_shader.set_vec3("viewPos", jugador->camara_jugador.posicion);

    // light properties
    if (jugador->torchlight_on)
    {
        suelo_shader.set_vec3("light.ambient", 0.05f, 0.05f, 0.05f);
        suelo_shader.set_vec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        suelo_shader.set_vec3("light.specular", 0.1f, 0.1f, 0.1f);

    }
    else 
    {
        suelo_shader.set_vec3("light.ambient", 0.005f, 0.005f, 0.005f);
        suelo_shader.set_vec3("light.diffuse", 0.0f, 0.0f, 0.0f);
        suelo_shader.set_vec3("light.specular", 0.0f, 0.0f, 0.0f);
    }

    suelo_shader.set_float("light.constant", 1.0f);
    suelo_shader.set_float("light.linear", 0.22f);
    suelo_shader.set_float("light.quadratic", 0.20f);

    //material properties
    suelo_shader.set_float("material.shininess", 32.0f);

    //torchlight mask
    suelo_shader.set_vec2("viewPort", contx.ancho_ventana, contx.alto_ventana);
    suelo_shader.set_int("light.flashlight", 2);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mascara_id);
}

void Escena_Juego::actualizacion_de_escena()
{
    reloj.actualizacion_de_escena();
    jugador->actualizacion_de_escena();
    enemy->actualizacion_de_escena();

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST); 

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    escena_shader.use();
    escena_shader.set_mat4("view", jugador->camara_jugador.get_view_matrix());
    escena_shader.set_mat4("projection", jugador->camara_jugador.get_projection_matrix());

    configuraciones_shaders();
    mapa.render(mapa_shader, suelo_shader, jugador->camara_jugador);
    enemy->render(mapa_shader, jugador->camara_jugador);

   
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); 
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    pantalla_shader.use();
    pantalla_shader.set_float("time", reloj.tiempo_actual);

    float distance = glm::distance(mapa.posicion_jugador, mapa.posicion_enemigo);

    float min_distance = 4.0f;
    float max_distance = 7.0f;

    float clamped_distance = captura(distance, min_distance, max_distance);
    float normalized_distance = rango_mapa(clamped_distance, min_distance, max_distance, 0.0f, 1.0f);

    float noise_intensity = rango_mapa(1.0f - normalized_distance, 0.0f, 1.0f, 0.0f, 0.3f);

    if (enemy->jugador_cerca)
    {
        pantalla_shader.set_float("noise_intensity", noise_intensity);
    } else {
        pantalla_shader.set_float("noise_intensity", 0.0f);
    }

    glBindVertexArray(quadvao);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    final_condicion();
}

void Escena_Juego::final_condicion()
{
    if (jugador->victory)
    {
        contx.cargar_escena_id(2);
        jugador->radio->game_over();
    }

    if (enemy->jugador_visto && jugador->torchlight_on)
    {
        jugador->dead = true;
    }

    if (jugador->dead)
    {
        susto();

        if (reloj.tiempo_actual - scream_start_time >= 3.0f) 
        {
            ma_sound_stop(&sonido_grito);
            contx.cargar_escena_id(2);
        }
    }
}

void Escena_Juego::susto()
{
    if (susto_activacion == false)
    {
        susto_activacion = true;
        scream_start_time = reloj.tiempo_actual;
        enemy->susto(*jugador);
        ma_sound_seek_to_pcm_frame(&sonido_grito, 0);
        ma_sound_start(&sonido_grito);
    }
}

void Escena_Juego::limpieza_de_escena()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Escena_Juego::proceso_de_entrada()
{
    bool k_pressed = false;

    if (glfwGetKey(contx.ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        contx.cargar_escena_id(2);
    }

    if (susto_activacion)
        return;

    if (glfwGetKey(contx.ventana, GLFW_KEY_W) == GLFW_PRESS) {
        k_pressed = true;
        jugador->process_keyboard(FORWARD, reloj.tiempo_final, k_pressed);
    }
    if (glfwGetKey(contx.ventana, GLFW_KEY_S) == GLFW_PRESS) {
        k_pressed = true;
        jugador->process_keyboard(BACKWARD, reloj.tiempo_final, k_pressed);
    }
    if (glfwGetKey(contx.ventana, GLFW_KEY_A) == GLFW_PRESS) {
        k_pressed = true;
        jugador->process_keyboard(LEFT, reloj.tiempo_final, k_pressed);
    }
    if (glfwGetKey(contx.ventana, GLFW_KEY_D) == GLFW_PRESS) {
        k_pressed = true;
        jugador->process_keyboard(RIGHT, reloj.tiempo_final, k_pressed);
    }

    jugador->update_velocity(k_pressed);
}

void Escena_Juego::llamado_mouse(GLFWwindow* ventana, double xposIn, double yposIn) 
{
    if (susto_activacion)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (mouse_firstview)
    {
        lastX = xpos;
        lastY = ypos;
        mouse_firstview = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    jugador->camara_jugador.process_mouse_movement(xoffset, yoffset);
}

void Escena_Juego::click_izquierdo(GLFWwindow* ventana, int button, int action, int mods)
{
    if (susto_activacion)
        return;

    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ma_engine_play_sound(&contx.centro_sonido.engine, "../assets/sfx/torchlight_click.wav", NULL);
        if (jugador->torchlight_on)
            jugador->torchlight_on = false;
        else
            jugador->torchlight_on = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (jugador->radio->radio_on)
            jugador->radio->turn_off();
        else
            jugador->radio->turn_on();
    }
}

void Escena_Juego::scroll(GLFWwindow* ventana, double xoffset, double yoffset) 
{
    jugador->camara_jugador.process_mouse_scroll(static_cast<float>(yoffset));
}

void Escena_Juego::llamado_tamaño_framebuffer(GLFWwindow* ventana, int ancho, int alto)
{
    glViewport(0, 0, ancho, alto);
    jugador->camara_jugador.ancho = ancho;
    jugador->camara_jugador.alto = alto;

    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);
    inicio_framebuffer();
}