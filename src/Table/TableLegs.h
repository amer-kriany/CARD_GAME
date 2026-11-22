#pragma once

#include <glm/glm.hpp>

#include "Graphics/Cube.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

// Four legs and a thick underside (apron) for the table.
// Everything here is built directly in WORLD space (already scaled),
// so draw it with the "world" uniform set to identity.
class TableLegs {
public:
    void draw(const Shader& shader, const Texture& woodTexture) const;

private:
    Cube cube_;

    void drawBox(
        const Shader& shader,
        const glm::vec3& center,
        const glm::vec3& size
    ) const;
};