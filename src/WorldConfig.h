#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shared numbers for the 3D scene, so main.cpp and CardAnimator agree.
namespace world
{
   
    constexpr float TABLE_SCALE = 2.5f;

    constexpr float TABLE_WIDTH = 1.8f;
    constexpr float TABLE_DEPTH = 1.4f;

    constexpr float FLOOR_Y = -1.5f;

    constexpr float ROOM_HALF = 5.0f;
    constexpr float CEILING_Y = 4.0f;

   
    inline glm::mat4 tableToWorld()
    {
        glm::mat4 m(1.0f);

        m = glm::scale(m, glm::vec3(TABLE_SCALE));

        m = glm::rotate(
            m,
            glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        return m;
    }
}