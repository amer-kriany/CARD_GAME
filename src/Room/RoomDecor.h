#pragma once

#include <glm/glm.hpp>

#include "Graphics/Cube.h"
#include "Graphics/Shader.h"


class RoomDecor {
public:
    void draw(const Shader& shader, float time) const;

private:
    Cube cube_;

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