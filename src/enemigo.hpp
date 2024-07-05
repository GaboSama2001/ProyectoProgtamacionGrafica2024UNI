#pragma once

#include "modelo.hpp"
#include "reloj.hpp"
#include "mapa.hpp"
#include "respiracion.hpp"
#include "miscelaneos.hpp"

class Enemigo
{
    enum Movimiento_Enemigo {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

public:
    bool grito = false;
    bool jugador_visto = false;
    bool jugador_cerca = false;

    Enemigo(Mapa& mapa, Sonido& sm) : mapa(mapa)
    {
        stbi_set_flip_vertically_on_load(false);
        ma_sound_init_from_file(&sm.engine, sf, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &sm.fence, &ruido);
        model = Modelo("../assets/models/enemy/monster.obj");

        model.transform.posicion = mapa.posicion_inicial_enemigo;
        model.transform.posicion.y += 0.3;
        model.transform.scale *= 0.1f;

        path_pos = tile_pos(model.transform.posicion);
    }

    void render(Shader shader, Camara3D& camara)
    {
        model.draw(shader, camara);
    }

    void init()
    {
        reloj.reset();
        front = glm::vec3(0.0f, 0.0f, 1.0f);
        model.transform.posicion = mapa.posicion_inicial_enemigo;
        model.transform.rotation.y = 0.0f;
        elegir_direccion = false;
        jugador_visto = false;
        jugador_cerca = false;
        grito = false;
        velocidad_movimiento = 0.3f;
        it = 0;
        path_pos = tile_pos(model.transform.posicion);
        path.clear();
        path = respiracion(mapa, path_pos, mapa.random_walkable_pos());
        if (path.empty())
        {
            path = respiracion(mapa, path_pos, mapa.random_walkable_pos());
        }
        ma_sound_start(&ruido);
        ma_sound_set_looping(&ruido, true);
    }

    void actualizacion_de_escena()
    {
        reloj.actualizacion_de_escena();
        update_sound_position();

        if (grito == true)
        {
            float distan = glm::distance(model.transform.posicion, mapa.posicion_jugador);
            if (distan >= 0.3f)
            {
                avanzar();
            }
            return;
        }

        if (it < path.size() - 1) {
            compute_direction();
        }
        else {
            path = respiracion(mapa, path_pos, mapa.random_walkable_pos());
            if (path.empty())
            {
                path = respiracion(mapa, path_pos, mapa.random_walkable_pos());
            }
            elegir_direccion = false;
            it = 0;
        }

        mapa.posicion_enemigo = model.transform.posicion;
        detect_player();
    }

    void susto(Jugador& jugador)
    {
        jugador.torchlight_on = true;
        model.transform.posicion = jugador.camara_jugador.posicion + jugador.camara_jugador.front * 3.0f;
        model.transform.posicion.y -= 0.2f;

        float distan = glm::distance(model.transform.posicion, jugador.camara_jugador.posicion);
        glm::vec3 direction_to_player = glm::normalize(jugador.camara_jugador.posicion - model.transform.posicion);
        glm::mat4 rotation_matrix = glm::lookAt(glm::vec3(0.0f), direction_to_player, glm::vec3(0.0f, 1.0f, 0.0f));
        float angle = atan2(rotation_matrix[0][2], rotation_matrix[2][2]) - M_PI;
        model.transform.rotation.y = angle;
        front = -jugador.camara_jugador.front;

        velocidad_movimiento = 4.0f;
        grito = true;
    }

    glm::ivec2 tile_pos(glm::vec3 pos)
    {
        return { int(std::round(pos.z / 1)) * 1, int(std::round(pos.x / 1)) * 1 };

    }

    void detect_player()
    {
        glm::ivec2 tp = tile_pos(mapa.posicion_jugador);
        glm::ivec2 tpe = tile_pos(mapa.posicion_enemigo);

        if (glm::distance(mapa.posicion_jugador, mapa.posicion_enemigo) > 9.0f)
        {
            jugador_cerca = false;
            jugador_visto = false;
            return;
        }

        std::vector<glm::ivec2> p = respiracion(mapa, path_pos, tile_pos(mapa.posicion_jugador));

        for (int i = 0; i < p.size(); i++)
        {
            if (p[i] == tile_pos(mapa.posicion_jugador))
            {
                jugador_cerca = true;
            }

            if (i < 4 && p[i] == tile_pos(mapa.posicion_jugador))
            {
                jugador_visto = true;
            }
            else { jugador_visto = false; }
        }
    }

private:
    Reloj reloj;
    Mapa& mapa;
    Modelo model;
    ma_sound ruido;
    const char* sf = "../assets/sfx/enemy_noise.wav";

    glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 derec = glm::vec3(-1.0f, 0.0f, 0.0f);

    std::vector<glm::ivec2> path;
    glm::ivec2 path_pos;

    float velocidad_movimiento = 0.3f;
    float velocidad;

    int it = 0;
    bool elegir_direccion = false;

    void avanzar()
    {
        velocidad = velocidad_movimiento * reloj.tiempo_final;
        model.transform.posicion += front * velocidad;
    }

    bool on_tile(glm::ivec2 path_tile)
    {
        float offset = 0.01f;
        float minimo_x = path_tile.y - offset;
        float maximo_x = path_tile.y + offset;
        float minimo_z = path_tile.x - offset;
        float maximo_z = path_tile.x + offset;

        if (model.transform.posicion.x >= minimo_x && model.transform.posicion.x <= maximo_x && model.transform.posicion.z >= minimo_z && model.transform.posicion.z <= maximo_z)
        {
            elegir_direccion = false;
            path_pos = path_tile;
            return true;
        }
        return false;
    }

    void cambiar_direccion(Movimiento_Enemigo direccion)
    {
        if (direccion == BACKWARD)
        {
            model.transform.rotation.y += glm::radians(180.0f);
            front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
        }
        if (direccion == LEFT)
        {
            model.transform.rotation.y += glm::radians(90.0f);
            front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
        }
        if (direccion == RIGHT)
        {
            model.transform.rotation.y += glm::radians(-90.0f);
            front = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f));
        }
    }

    void compute_direction()
    {
        glm::vec3 forward = glm::normalize(front);
        glm::vec2 DireccionActual = { forward.z, forward.x };
        glm::vec2 direccion = (path[it] - path_pos);

        path_pos = tile_pos(model.transform.posicion);

        if (on_tile(path[it])) {
            it++;
            elegir_direccion = false;
            avanzar();
        }
        else {
            avanzar();
        }

        if (elegir_direccion == false) {
            elegir_direccion = true;
            direccion = (path[it] - path_pos);
            DireccionActual = { forward.z, forward.x };

            float dotProduct = glm::dot(direccion, DireccionActual);

            if (dotProduct < -0.95f) {
                cambiar_direccion(BACKWARD);
            }
            else if (dotProduct > 0.95f) {
            }
            else {
                float crossProduct = direccion.x * DireccionActual.y - direccion.y * DireccionActual.x;
                if (crossProduct > 0.0f) {
                    cambiar_direccion(RIGHT);
                }
                else {
                    cambiar_direccion(LEFT);
                }
            }
        }
    }

    void update_sound_position()
    {
        ma_sound_set_position(&ruido, model.transform.posicion.x, model.transform.posicion.y, model.transform.posicion.z);
        float distance = glm::distance(mapa.posicion_jugador, model.transform.posicion);

        float min_distance = 4.0f;
        float max_distance = 7.0f;

        float clamped_distance = captura(distance, min_distance, max_distance);
        float normalized_distance = rango_mapa(clamped_distance, min_distance, max_distance, 0.0f, 1.0f);

        float volume = rango_mapa(normalized_distance, 1.0f, 0.0f, 0.0f, 12.0f);
        ma_sound_set_volume(&ruido, volume);
    }
};