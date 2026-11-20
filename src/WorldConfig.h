#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shared numbers for the 3D scene, so main.cpp and CardAnimator agree.
//
// World space (Fadi's cameras assume this):
//   +X = right, +Y = up, +Z = toward the player.
//   The table top is the plane Y = 0.
//
// Table space (what Table.cpp and the zones still use):
//   +X = right, +Y = toward the enemy, Z = height above the table.
namespace world
{
    // Our table is 1.8 x 1.4 units, Fadi's cameras sit several units away
    // and the room is ~11 units wide, so everything is scaled up.
    constexpr float TABLE_SCALE = 2.5f;

    // Table top size in table units (matches Table's size_ of 1.8 x 1.4).
    constexpr float TABLE_WIDTH = 1.8f;
    constexpr float TABLE_DEPTH = 1.4f;

    // The table top is at y = 0, so the floor of the room is below it.
    constexpr float FLOOR_Y = -1.5f;

    // Turns the old XY table into a table lying on the XZ floor plane.
    // (x, y, z) -> (x, z, -y), then scaled.
    //   deck  zone (y = -0.48) -> z = +1.2  (near the player)
    //   enemy zone (y = +0.48) -> z = -1.2  (far from the player)
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