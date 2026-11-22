#pragma once

#include <glm/glm.hpp>

#include "Graphics/Cube.h"
#include "Graphics/Shader.h"

// Fadi's three decor pieces, ported to our classes and fitted to the room:
//   1. a bookshelf against the left wall
//   2. a hanging Saturn model near the back right corner
// (a pendulum wall clock is also written, but not drawn: the adventurer
//  model in main.cpp is the third piece, and the poster must stay visible)
// Every piece is built only from scaled Cubes.
class RoomDecor {
public:
    // time = seconds since start (drives the pendulum and the spinning ring).
    void draw(const Shader& shader, float time) const;

private:
    Cube cube_;

    // Draws one box: "center" and "size" are in the parent's space.
    void box(
        const Shader& shader,
        const glm::mat4& parent,
        const glm::vec3& center,
        const glm::vec3& size,
        const glm::vec3& color
    ) const;

    void drawClock(const Shader& shader, float time) const;
    void drawBookshelf(const Shader& shader) const;
    void drawSaturn(const Shader& shader, float time) const;
};