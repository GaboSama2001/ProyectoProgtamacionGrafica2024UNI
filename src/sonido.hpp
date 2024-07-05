#pragma once

#include <miniaudio.h>
#include <vector>

class Sonido {
    public:
        ma_engine engine;
        ma_result result;
        ma_fence fence;

        ma_spatializer_listener listener;

        std::vector<ma_sound> sounds;

        Sonido()
        {
            result = ma_engine_init(NULL, &engine);
            if (result != MA_SUCCESS) {
                printf("No se pudo inicializar el motor de audio.");
                return;
            }

            result = ma_fence_init(&fence);
            if (result != MA_SUCCESS) {
                std::cout << "No se puede inicializar la valla para los sonidos de radio.";
            }
        }
};