#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class controlador_camara {
    public:
        controlador_camara(){} 
        virtual ~controlador_camara(){}
        virtual glm::mat4 get_view_matrix() const = 0;
        virtual glm::mat4 get_projection_matrix() const = 0;
};