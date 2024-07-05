#pragma once

#include "mapa.hpp"
#include <queue>
#include <unordered_map>
#include "glm/gtx/hash.hpp"

inline bool is_valid(const std::vector<std::vector<char>> &mapa_txt, int x, int y)
{
    int rows = mapa_txt.size();
    int cols = mapa_txt[0].size();
    return (x >= 0 && x < rows && y >= 0 && y < cols && mapa_txt[x][y] == ' ');
}

inline std::vector<glm::ivec2> respiracion(Mapa &mapa, glm::ivec2 posicion_resini, glm::ivec2 posicion_final)
{
    std::queue<glm::ivec2> frontier;
    std::unordered_map<glm::ivec2, glm::ivec2> came_from;

    frontier.push(posicion_resini);
    glm::ivec2 furthest = posicion_resini;

    do {
        glm::ivec2 actual = frontier.front();
        frontier.pop();

        if (actual == posicion_final)
        {
            break;
        }

        const std::vector<glm::ivec2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        bool found_next = false;

        for (int i = 0; i < directions.size(); i++)
        {
            glm::ivec2 next(actual + directions[i]);

            if (!is_valid(mapa.mapa_txt, next.x, next.y))
                continue;

            if (came_from.find(next) == came_from.end()) 
            {
                frontier.push(next);
                came_from[next] = actual;
                found_next = true;
            }
        }
    } while (!frontier.empty());

    glm::ivec2 actual = posicion_final;
    std::vector<glm::ivec2> path;

    if (came_from.find(posicion_final) == came_from.end())
        return path;

    while(actual != posicion_resini)
    {
        path.push_back(actual);
        actual = came_from[actual];
    }

    std::reverse(path.begin(), path.end());
    return path;
}