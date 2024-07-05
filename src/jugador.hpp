#include "camara3d.hpp"
#include "modelo.hpp"
#include "mapa.hpp"
#include "radio.hpp"

class Jugador
{
    public:
        Camara3D camara_jugador;

        Modelo torchlight;
        Mapa &mapa;

        bool step = false;
        bool victory = false;
        bool dead = false;

        Radio *radio;

        bool torchlight_on = true;

        Jugador(Mapa &mapa, Sonido &centro_sonido, float ancho_ventana = 800, float alto_ventana = 600) : mapa(mapa), centro_sonido(centro_sonido)
        {
            cargar_sonido();

            ma_fence_wait(&centro_sonido.fence);
            camara_jugador = Camara3D(mapa.posicion_jugador, 860.0f, 520.0f, 1.0f, true);
            radio = new Radio(centro_sonido, mapa.posicion_jugador, mapa.posicion_ventana);
        }

        void init()
        {//validacion del estado del jugador
            victory = false;
            radio->muerte_jugador = false;
            radio->tiempo_escucha = 0.0f;
            radio->numero_activacion = 0;
            camara_jugador.posicion = mapa.posicion_inicial_jugador;
            torchlight_on = true;
            mapa.posicion_jugador = mapa.posicion_inicial_jugador;
        }

        void actualizacion_de_escena()
        {
            reloj.actualizacion_de_escena();
            radio->actualizacion_de_escena();
            if (velocidad > 0.0f)
            {
                camara_jugador.posicion.y = headbob(reloj.tiempo_final, reloj.tiempo_actual) + camara_jugador.posicion_inicial.y;
            }

            victoria();
            muerte();
        }

        void update_velocity(bool k_pressed)
        {
            if (!k_pressed) {
                velocidad = 0.0f;
            } else {
                velocidad = camara_jugador.velocidad_movimiento * reloj.tiempo_final;
            }
        }

        void process_keyboard(Movimiento_Camara3D direccion, float tiempo_final, bool k_pressed)
        {
            if (collide(direccion))
            {
                velocidad = 0 * tiempo_final;
            } else {
               velocidad = camara_jugador.velocidad_movimiento * tiempo_final; 
            }

            if (direccion == FORWARD)
                camara_jugador.posicion += camara_jugador.front * velocidad;
            if (direccion == BACKWARD)
                camara_jugador.posicion -= camara_jugador.front * velocidad;
            if (direccion == LEFT)
                camara_jugador.posicion -= camara_jugador.derec * velocidad;
            if (direccion == RIGHT)
                camara_jugador.posicion += camara_jugador.derec * velocidad;
            if (camara_jugador.fps)
                camara_jugador.posicion.y = camara_jugador.posicion_inicial.y;

            mapa.posicion_jugador = camara_jugador.posicion;
        }

    private:
        Reloj reloj;
        Sonido &centro_sonido;
        ma_sound step_sounds[8];
        float velocidad;

        const char *sound_files[8] = {
            "../assets/sfx/footsteps/step1.wav",
            "../assets/sfx/footsteps/step2.wav",
            "../assets/sfx/footsteps/step3.wav",
            "../assets/sfx/footsteps/step4.wav",
            "../assets/sfx/footsteps/step5.wav",
            "../assets/sfx/footsteps/step7.wav",
            "../assets/sfx/footsteps/step8.wav",
            "../assets/sfx/footsteps/step10.wav"
        };

        float headbob(float tiempo_final, float tiempo_actual)
        {
            float frecuencia_de_cabeceo = 0.7f;
            float cabeceo_y = 0.03f;
            
            float bobbing = glm::abs(glm::sin(glm::pi<float>() * (tiempo_actual / frecuencia_de_cabeceo))) * cabeceo_y;

            if (bobbing >= 0.005 && step)
            {
                step = false;
            }
            if (bobbing <= 0.005 && !step) {
                step = true;
                int random = random_int(0, 7);
                ma_sound_seek_to_pcm_frame(&step_sounds[random], 0);
                ma_sound_start(&step_sounds[random]);
            }

            return bobbing;
        }

        void victoria()
        {//confimacion de las victorio
            float offset = 0.11f;
            float minimo_x = mapa.posicion_ventana.x - 0.4f - offset;
            float maximo_x = mapa.posicion_ventana.x + 0.4f + offset;
            float minimo_z = mapa.posicion_ventana.z - 0.4f - offset;
            float maximo_z = mapa.posicion_ventana.z + 0.4f + offset;
    
            if (camara_jugador.posicion.x >= minimo_x && camara_jugador.posicion.x <= maximo_x && camara_jugador.posicion.z >= minimo_z && camara_jugador.posicion.z <= maximo_z)
            {
                victory = true;
            }
        }

        void muerte()
        {
            dead = radio->muerte_jugador;
        }

        bool collide(Movimiento_Camara3D direccion)
        {
            glm::vec3 sig_posicion = camara_jugador.posicion;
            if (direccion == FORWARD)
                sig_posicion += camara_jugador.front * velocidad;
            if (direccion == BACKWARD)
                sig_posicion -= camara_jugador.front * velocidad;
            if (direccion == LEFT)
                sig_posicion -= camara_jugador.derec * velocidad;
            if (direccion == RIGHT)
                sig_posicion += camara_jugador.derec * velocidad;

            for (auto &posicion_muro : mapa.posicion_muro)
            {
                float offset = 0.11f;
                float minimo_x = posicion_muro.x - 0.5f - offset;
                float maximo_x = posicion_muro.x + 0.5f + offset;
                float minimo_z = posicion_muro.z - 0.5f - offset;
                float maximo_z = posicion_muro.z + 0.5f + offset;

                if (sig_posicion.x >= minimo_x && sig_posicion.x <= maximo_x && sig_posicion.z >= minimo_z && sig_posicion.z <= maximo_z)
                {
                    return true;
                }
            }
            return false;
        }

        void cargar_sonido()
        {
            for (int i = 0; i < 8; i += 1)
            {
                ma_sound_init_from_file(&centro_sonido.engine, sound_files[i], MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &centro_sonido.fence, &step_sounds[i]);
            }
        }

        int random_int(int minimo, int maximo)
        {
            assert(maximo > minimo); 
            return (rand() % (maximo - minimo + 1) + minimo);
        }
};