#include "miscelaneos.hpp"


float rango_mapa(float value, float min1, float max1, float min2, float max2)
{
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float captura(float n, float lower, float upper)
{
    return n <= lower ? lower : n >= upper ? upper : n;
}