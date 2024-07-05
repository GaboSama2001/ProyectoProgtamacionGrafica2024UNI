#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "formas.hpp"
#include "cielo.hpp"

class Mapa {
public:
    std::vector<std::vector<char>> mapa_txt;
    std::vector<glm::vec3> posicion_muro;
    glm::vec3 posicion_jugador = { 0.0f, 0.5f, 0.0f };
    glm::vec3 posicion_inicial_jugador = { 0.0f, 0.5f, 0.0f };
    glm::vec3 posicion_ventana;
    glm::vec3 posicion_estatua1;
    glm::vec3 posicion_estatua2;
    glm::vec3 posicion_estatua3;
    glm::vec3 posicion_estatua4;
    glm::vec3 posicion_enemigo;
    glm::vec3 posicion_inicial_enemigo;

    Mapa() {
        lectura_mapa_file("../assets/final_map.txt");
        stbi_set_flip_vertically_on_load(false);
        estatua = Modelo("../assets/models/statue/Forest Skull.OBJ");
        estatua2 = Modelo("../assets/models/statue2/model.obj");
        estatua3 = Modelo("../assets/models/statue3/12140_Skull_v3_L2.obj");
        estatua4 = Modelo("../assets/models/statue4/12337_Statue_v1_l1.obj");
        brother = Modelo("../assets/models/brother/maya2sketchfab.obj");
        stbi_set_flip_vertically_on_load(true);
        muro = Modelo("../assets/models/wall/wall.obj");
        jaula = Modelo("../assets/models/cage/ai-texturing-hest-low-poly/source/chest.obj");

        // Initialize Skybox
        skybox = new Skybox();
    }

    ~Mapa() {
        delete skybox;
    }

    void render(Shader shader, Shader shader2, const controlador_camara& camara) {
        // Render the skybox first
        skybox->render(camara);

        // Existing render code...
        for (auto& pos : posicion_muro) {
            muro.transform.posicion = pos;
            muro.draw(shader, camara);
        }

        jaula.draw(shader, camara);
        estatua.draw(shader, camara);
        estatua2.draw(shader, camara);
        estatua3.draw(shader, camara);
        estatua4.draw(shader, camara);
        brother.draw(shader, camara);

        floor.transform.posicion = floor_position;
        floor.render(shader2, camara);
        floor.transform.posicion = roof_position;
        floor.render(shader2, camara);

        glm::vec3 direction_to_player = glm::normalize(posicion_jugador - posicion_estatua1);
        float angle = atan2(direction_to_player.x, direction_to_player.z) - M_PI / 2.0f;
        estatua.transform.rotation.y = angle;

        glm::vec3 direction_to_player2 = glm::normalize(posicion_jugador - posicion_estatua2);
        float angle2 = atan2(direction_to_player2.x, direction_to_player2.z) - M_PI / 2.0f;
        estatua2.transform.rotation.y = angle2;

        glm::vec3 direction_to_player3 = glm::normalize(posicion_jugador - posicion_estatua3);
        float angle3 = atan2(direction_to_player3.x, direction_to_player3.z) - M_PI;
        estatua3.transform.rotation.y = angle3;

        glm::vec3 direction_to_player4 = glm::normalize(posicion_jugador - posicion_estatua4);
        float angle4 = atan2(direction_to_player4.x, direction_to_player4.z) - M_PI;
        estatua4.transform.rotation.y = angle4;
    }

    void load_map() {
        glm::vec3 posicion = { 0.0f, 0.0f, 0.0f };
        floor.add_texture("../assets/textures/concrete2.jpg", floor.textura_difusa);
        floor.transform.scale.x *= mapa_txt[0].size();
        floor.transform.scale.z *= mapa_txt.size();
        floor.transform.scale.y *= 0.1f;
        floor.transform.posicion.x += mapa_txt[0].size() / 2;
        floor.transform.posicion.z += mapa_txt.size() / 2;
        floor_position = { floor.transform.posicion.x, floor.transform.posicion.y, floor.transform.posicion.z };

        for (const auto& row : mapa_txt) {
            for (const auto& element : row) {
                switch (element) {
                case '#':
                    posicion_muro.push_back(posicion);
                    break;
                case '@':
                    posicion_inicial_jugador = { posicion.x, posicion_jugador.y, posicion.z };
                    break;
                case 'w':
                    posicion_ventana = posicion;
                    break;
                case 'A':
                    posicion_estatua1 = { posicion.x, 0.03f, posicion.z };
                    break;
                case 'B':
                    posicion_estatua2 = { posicion.x, 0.30f, posicion.z };
                    break;
                case 'C':
                    posicion_estatua3 = { posicion.x, 0.1f, posicion.z };
                    break;
                case 'D':
                    posicion_estatua4 = { posicion.x, 0.05f, posicion.z };
                    break;        
                case '&':
                    posicion_inicial_enemigo = { posicion.x, 0.3, posicion.z };
                    break;
                }
                posicion.x += 1.0f;
            }
            posicion.x = 0.0f;
            posicion.z += 1.0f;
        }

        jaula.transform.posicion = posicion_ventana;
        jaula.transform.scale *= 0.003f;

        brother.transform.posicion = posicion_ventana;
        brother.transform.posicion.y = 0.06;
        brother.transform.scale *= 0.004f;

        estatua.transform.posicion = posicion_estatua1;
        estatua.transform.scale *= 0.1f;

        estatua2.transform.posicion = posicion_estatua2;
        estatua2.transform.scale *= 0.7f;

        estatua3.transform.posicion = posicion_estatua3;
        estatua3.transform.scale *= 0.03f;

        estatua4.transform.posicion = posicion_estatua4;
        estatua4.transform.scale *= 0.0006f;

    }

    glm::ivec2 random_walkable_pos() {
        srand(time(NULL));
        int rows = mapa_txt.size();
        int cols = mapa_txt[0].size();
        int x, y;
        do {
            x = (rand() % rows);
            y = (rand() % cols);
        } while (mapa_txt[x][y] != ' ');

        return glm::ivec2{ x, y };
    }

private:
    Modelo jaula;
    Modelo muro;
    Modelo estatua;
    Modelo estatua2;
    Modelo estatua3;
    Modelo estatua4;
    Modelo brother;

    Cubo floor;
    glm::vec3 floor_position;
    glm::vec3 roof_position;
    Shader suelo_shader;

    Skybox* skybox;

    void lectura_mapa_file(const char* path) {
        std::string line;
        std::ifstream infile(path);

        if (!infile.is_open()) {
            std::cout << "can't open file" << std::endl;
            exit(-1);
        }

        while (std::getline(infile, line)) {
            std::vector<char> row;

            for (char& c : line) {
                row.push_back(c);
            }

            mapa_txt.push_back(row);
        }
    }
};
