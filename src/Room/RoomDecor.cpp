#include "RoomDecor.h"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "WorldConfig.h"

namespace
{
    // basic.frag material 6 = solid color with simple lighting.
    constexpr int LIT_COLOR = 6;

    const glm::vec3 WOOD(0.35f, 0.18f, 0.08f);
    const glm::vec3 LIGHT_WOOD(0.45f, 0.26f, 0.13f);
    const glm::vec3 CREAM(0.95f, 0.92f, 0.85f);
    const glm::vec3 DARK(0.10f, 0.10f, 0.10f);
    const glm::vec3 GOLD(0.85f, 0.65f, 0.13f);
}

void RoomDecor::box(
    const Shader& shader,
    const glm::mat4& parent,
    const glm::vec3& center,
    const glm::vec3& size,
    const glm::vec3& color
) const
{
    // Same idea as TableLegs: Cube is a unit cube at the origin,
    // so scale it, move it, then place it inside its parent.
    glm::mat4 model = parent;
    model = glm::translate(model, center);
    model = glm::scale(model, size);

    shader.setMat4("model", model);
    shader.setVec4("tint", glm::vec4(color, 1.0f));

    cube_.draw();
}

void RoomDecor::draw(const Shader& shader, float time) const
{
    shader.use();

    // Decor is already in world space.
    shader.setMat4("world", glm::mat4(1.0f));
    shader.setInt("useTexture", LIT_COLOR);

    drawClock(shader, time);
    drawBookshelf(shader);
    // Saturn is no longer drawn: the adventurer model (see main.cpp) is the
    // third decor piece. Uncomment to bring Saturn back.
    // drawSaturn(shader, time);
}

// ---------------------------------------------------------------------
// 1. Pendulum clock, on the back wall (z = -ROOM_HALF), left of the poster.
// ---------------------------------------------------------------------
void RoomDecor::drawClock(const Shader& shader, float time) const
{
    const glm::mat4 none(1.0f);

    // The frame is 0.2 deep, so its center sits 0.1 in front of the wall.
    const glm::vec3 c(-3.4f, 1.0f, -world::ROOM_HALF + 0.1f);

    // Wooden frame.
    box(shader, none, c, glm::vec3(1.2f, 1.6f, 0.2f), WOOD);

    // White face.
    box(shader, none, c + glm::vec3(0.0f, 0.35f, 0.11f),
        glm::vec3(0.8f, 0.8f, 0.02f), CREAM);

    // Minute and hour hands.
    box(shader, none, c + glm::vec3(0.0f, 0.45f, 0.13f),
        glm::vec3(0.04f, 0.3f, 0.01f), DARK);
    box(shader, none, c + glm::vec3(0.12f, 0.35f, 0.13f),
        glm::vec3(0.25f, 0.04f, 0.01f), DARK);

    // Pendulum: everything inside is drawn relative to the pivot,
    // so rotating the pivot swings the whole thing.
    glm::mat4 pendulum(1.0f);
    pendulum = glm::translate(pendulum, c + glm::vec3(0.0f, -0.05f, 0.12f));
    pendulum = glm::rotate(
        pendulum,
        std::sin(time * 2.5f) * 0.35f,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    box(shader, pendulum, glm::vec3(0.0f, -0.35f, 0.0f),
        glm::vec3(0.06f, 0.7f, 0.02f), GOLD);
    box(shader, pendulum, glm::vec3(0.0f, -0.70f, 0.0f),
        glm::vec3(0.20f, 0.20f, 0.03f), GOLD);
}

// ---------------------------------------------------------------------
// 2. Bookshelf, against the left wall (x = -ROOM_HALF).
//    Fadi's version was one solid block with the books inside it, so the
//    books were invisible. This one is an open frame with books on shelves.
// ---------------------------------------------------------------------
void RoomDecor::drawBookshelf(const Shader& shader) const
{
    const glm::mat4 none(1.0f);

    const float depth = 0.6f;    // how far it sticks out of the wall (x)
    const float width = 2.2f;    // along the wall (z)
    const float height = 3.0f;
    const float board = 0.06f;

    const float wallX = -world::ROOM_HALF;
    const float centerZ = -3.7f;
    const float baseY = world::FLOOR_Y;

    const float midX = wallX + depth * 0.5f;
    const float midY = baseY + height * 0.5f;

    // Back panel against the wall.
    box(shader, none, glm::vec3(wallX + 0.025f, midY, centerZ),
        glm::vec3(0.05f, height, width), WOOD);

    // Two sides.
    for (int side = -1; side <= 1; side += 2)
    {
        box(shader, none,
            glm::vec3(midX, midY, centerZ + side * (width * 0.5f - board * 0.5f)),
            glm::vec3(depth, height, board), WOOD);
    }

    // Four boards: bottom, two middle, top.
    const float boardStep = (height - board) / 3.0f;

    for (int i = 0; i < 4; ++i)
    {
        box(shader, none,
            glm::vec3(midX, baseY + board * 0.5f + i * boardStep, centerZ),
            glm::vec3(depth, board, width), LIGHT_WOOD);
    }

    // Books standing on the three lower boards.
    const glm::vec3 bookColors[5] = {
        glm::vec3(0.70f, 0.15f, 0.15f),
        glm::vec3(0.15f, 0.35f, 0.65f),
        glm::vec3(0.18f, 0.55f, 0.25f),
        glm::vec3(0.80f, 0.62f, 0.15f),
        glm::vec3(0.50f, 0.25f, 0.60f)
    };

    const float bookWidth = 0.14f;
    const int booksPerShelf = 11;

    for (int shelf = 0; shelf < 3; ++shelf)
    {
        const float shelfTop = baseY + board + shelf * boardStep;

        for (int i = 0; i < booksPerShelf; ++i)
        {
            // Slightly different heights so it does not look like a wall.
            const float bookHeight = 0.55f + 0.08f * ((i * 3 + shelf) % 4);

            const float z = centerZ - width * 0.5f + board
                          + bookWidth * (i + 0.5f) + 0.02f * i;

            box(shader, none,
                glm::vec3(wallX + 0.05f + 0.2f, shelfTop + bookHeight * 0.5f, z),
                glm::vec3(0.4f, bookHeight, bookWidth),
                bookColors[(i + shelf * 2) % 5]);
        }
    }
}

// ---------------------------------------------------------------------
// 3. Saturn: a cube "planet" with a square ring, hanging from the ceiling
//    near the back right corner. The ring and planet slowly spin.
// ---------------------------------------------------------------------
void RoomDecor::drawSaturn(const Shader& shader, float time) const
{
    const glm::mat4 none(1.0f);

    const glm::vec3 center(3.6f, 1.2f, -3.8f);

    // String from the ceiling down to the planet.
    const float stringTop = world::CEILING_Y;
    const float stringBottom = center.y + 0.4f;

    box(shader, none,
        glm::vec3(center.x, (stringTop + stringBottom) * 0.5f, center.z),
        glm::vec3(0.02f, stringTop - stringBottom, 0.02f),
        glm::vec3(0.6f, 0.6f, 0.6f));

    // Everything below is relative to this matrix: move to the planet,
    // spin around the vertical axis, then tilt like Saturn.
    glm::mat4 saturn(1.0f);
    saturn = glm::translate(saturn, center);
    saturn = glm::rotate(saturn, time * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
    saturn = glm::rotate(saturn, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // Planet body.
    box(shader, saturn, glm::vec3(0.0f), glm::vec3(0.75f), glm::vec3(0.85f, 0.55f, 0.20f));

    // Ring: four thin bars forming a hollow square, 1.8 across.
    const glm::vec3 ringColor(0.75f, 0.68f, 0.50f);
    const float outer = 0.9f;
    const float bar = 0.22f;
    const float edge = outer - bar * 0.5f;

    box(shader, saturn, glm::vec3(0.0f, 0.0f,  edge),
        glm::vec3(outer * 2.0f, 0.04f, bar), ringColor);
    box(shader, saturn, glm::vec3(0.0f, 0.0f, -edge),
        glm::vec3(outer * 2.0f, 0.04f, bar), ringColor);
    box(shader, saturn, glm::vec3( edge, 0.0f, 0.0f),
        glm::vec3(bar, 0.04f, outer * 2.0f - bar * 2.0f), ringColor);
    box(shader, saturn, glm::vec3(-edge, 0.0f, 0.0f),
        glm::vec3(bar, 0.04f, outer * 2.0f - bar * 2.0f), ringColor);
}