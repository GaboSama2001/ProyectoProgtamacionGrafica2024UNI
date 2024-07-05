#pragma once

#include <glm/gtc/random.hpp>

#include "reloj.hpp"
#include "sonido.hpp"

class Radio 
{
    public:
        int numero_activacion;
        int maximo_activacion;
    
        float tiempo_escucha;
        float max_tiempo_escucha = 7.0f;

        bool radio_on;
        bool muerte_jugador;

        glm::vec3 &posicion_rjugador;
        glm::vec3 &posicion_rventana;

        Radio(Sonido &centro_sonido, glm::vec3 &posicion_rjugador, glm::vec3 &posicion_rventana) : centro_sonido(centro_sonido), posicion_rjugador(posicion_rjugador), posicion_rventana(posicion_rventana)
        {
            srand (time(NULL));

            cargar_sonido();
            muerte_jugador = false;
            radio_on = false;
            numero_activacion = 0;
            maximo_activacion = random_int(7, 10);
        };

        void actualizacion_de_escena()
        {
            if (numero_activacion > maximo_activacion)
                game_over();

            reloj.actualizacion_de_escena();
            if (radio_on)
            {
                tiempo_escucha += reloj.tiempo_final;
                if (tiempo_escucha > max_tiempo_escucha)
                {
                    game_over();
                }
            }
        }

        void turn_on()
        {

            if (glm::distance(posicion_rjugador, posicion_rventana) < 10.0f) 
            {
                ma_sound_seek_to_pcm_frame(&sonido_radio[0], 0);
                ma_sound_start(&sonido_radio[0]);
            }
            else if (glm::distance(posicion_rjugador, posicion_rventana) <= 30.0f) {
                ma_sound_seek_to_pcm_frame(&sonido_radio[1], 0);
                ma_sound_start(&sonido_radio[1]);
            }
            else {
                ma_sound_seek_to_pcm_frame(&sonido_radio[2], 0);
                ma_sound_start(&sonido_radio[2]);
            }
            numero_activacion += 1;
            radio_on = true;
        }

        void turn_off()
        {
            tiempo_escucha = 0.0f;
            radio_on = false;
            ma_sound_stop(&sonido_radio[0]);
            ma_sound_stop(&sonido_radio[1]);
            ma_sound_stop(&sonido_radio[2]);
        }

        void game_over()
        {
            muerte_jugador = true;
            turn_off();
        }
        
    private:
        Reloj reloj;
        Sonido &centro_sonido;
        

        ma_sound sonido_radio[3];
        const char *sound_files[3] = {
            "../assets/sfx/radio_ultra_near.wav",
            "../assets/sfx/radio_between.wav",
            "../assets/sfx/radio_far.wav"
        };

        void cargar_sonido()
        {
            for (int i = 0; i < 3; i += 1)
            {
                ma_sound_init_from_file(&centro_sonido.engine, sound_files[i], MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &centro_sonido.fence, &sonido_radio[i]);
            }
            ma_fence_wait(&centro_sonido.fence);
        }

        int random_int(int minimo, int maximo)
        {
            assert(maximo > minimo); 
            return (rand() % (maximo - minimo + 1) + minimo);
        }
};