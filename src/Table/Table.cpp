
#include "Table.h"

#include <glm/gtc/matrix_transform.hpp>

namespace
{
    const glm::vec4 kShadow(
        0.012f, 0.007f, 0.005f, 1.0f
    );

    const glm::vec4 kDarkWood(
        0.075f, 0.035f, 0.018f, 1.0f
    );

    const glm::vec4 kGold(
        1.0f, 0.82f, 0.48f, 1.0f
    );

    const glm::vec4 kGoldDark(
        0.48f, 0.29f, 0.10f, 1.0f
    );

    const glm::vec4 kNormalLeather(
        0.48f, 0.20f, 0.09f, 1.0f
    );

    const glm::vec4 kGameLeather(
        0.12f, 0.24f, 0.53f, 1.0f
    );

    void drawMaterial(
        const Shader& shader,
        const Quad& quad,
        const glm::vec2& center,
        const glm::vec2& dimensions,
        const glm::vec4& color,
        int material,
        float rotation = 0.0f
    )
    {
        glm::mat4 model(1.0f);

        model = glm::translate(
            model,
            glm::vec3(center.x, center.y, 0.0f)
        );

        model = glm::rotate(
            model,
            rotation,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        model = glm::scale(
            model,
            glm::vec3(dimensions.x, dimensions.y, 1.0f)
        );

        shader.setMat4("model", model);
        shader.setInt("useTexture", material);
        shader.setVec4("tint", color);

        quad.draw();
    }

    void drawCorner(
        const Shader& shader,
        const Quad& quad,
        const glm::vec2& position,
        float rotation
    )
    {
        drawMaterial(
            shader, quad,
            position,
            glm::vec2(0.105f, 0.105f),
            kGoldDark,
            3,
            rotation
        );

        drawMaterial(
            shader, quad,
            position,
            glm::vec2(0.065f, 0.065f),
            kGold,
            3,
            rotation
        );

        drawMaterial(
            shader, quad,
            position,
            glm::vec2(0.025f, 0.025f),
            kDarkWood,
            0,
            rotation
        );
    }

    void drawZone(
        const Shader& shader,
        const Quad& quad,
        const Zone& zone,
        bool isGameZone
    )
    {
        const glm::vec4 leatherTint =
            isGameZone ? kGameLeather : kNormalLeather;

        const float border = isGameZone ? 0.025f : 0.020f;

        // Outer shadow.
        drawMaterial(
            shader, quad,
            zone.center + glm::vec2(0.0f, -0.008f),
            zone.size + glm::vec2(0.045f),
            kShadow,
            0
        );

        drawMaterial(
            shader, quad,
            zone.center,
            zone.size + glm::vec2(border * 2.0f),
            kGoldDark,
            3
        );

        drawMaterial(
            shader, quad,
            zone.center,
            zone.size + glm::vec2(0.012f),
            kGold,
            3
        );

        // Leather surface.
        drawMaterial(
            shader, quad,
            zone.center,
            zone.size,
            leatherTint,
            4
        );

        // Small dark inner border.
        drawMaterial(
            shader, quad,
            zone.center,
            zone.size - glm::vec2(0.018f),
            kDarkWood,
            0
        );

        // Final leather surface.
        drawMaterial(
            shader, quad,
            zone.center,
            zone.size - glm::vec2(0.030f),
            leatherTint,
            4
        );

        const float x = zone.size.x * 0.5f - 0.035f;
        const float y = zone.size.y * 0.5f - 0.035f;

        const glm::vec2 studs[4] = {
            zone.center + glm::vec2(-x, -y),
            zone.center + glm::vec2( x, -y),
            zone.center + glm::vec2(-x,  y),
            zone.center + glm::vec2( x,  y)
        };

        for (const glm::vec2& stud : studs)
        {
            drawMaterial(
                shader, quad,
                stud,
                glm::vec2(0.018f, 0.018f),
                kGold,
                3,
                glm::radians(45.0f)
            );
        }
    }
}

Table::Table(const std::string& woodTexturePath)
    : wood_(woodTexturePath)
{
    zones_[2] = {
        "enemy",
        glm::vec2(0.0f, 0.48f),
        glm::vec2(1.70f, 0.32f),
        glm::vec4(1.0f)
    };

    zones_[1] = {
        "game",
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.70f, 0.58f),
        glm::vec4(1.0f)
    };

    zones_[0] = {
        "deck",
        glm::vec2(0.0f, -0.48f),
        glm::vec2(1.70f, 0.32f),
        glm::vec4(1.0f)
    };
}

void Table::draw(
    const Shader& shader,
    const Quad& quad
) const
{
    shader.use();

    // 1. Soft shadow beneath the table.
    drawMaterial(
        shader, quad,
        glm::vec2(0.0f, -0.018f),
        glm::vec2(1.88f, 1.48f),
        kShadow,
        0
    );

    // 2. Outer antique-gold frame.
    drawMaterial(
        shader, quad,
        glm::vec2(0.0f),
        glm::vec2(1.86f, 1.46f),
        kGoldDark,
        3
    );

    // 3. Main dark wooden frame.
    drawMaterial(
        shader, quad,
        glm::vec2(0.0f),
        glm::vec2(1.82f, 1.42f),
        kDarkWood,
        0
    );

    glm::mat4 model(1.0f);

    model = glm::scale(
        model,
        glm::vec3(size_.x, size_.y, 1.0f)
    );

    shader.setMat4("model", model);
    shader.setInt("useTexture", 2);
    shader.setVec4(
        "tint",
        glm::vec4(0.82f, 0.65f, 0.48f, 1.0f)
    );

    wood_.bind();
    quad.draw();

    const float outerX = 0.875f;
    const float outerY = 0.675f;
    const float thickness = 0.012f;

    // Top and bottom.
    drawMaterial(
        shader, quad,
        glm::vec2(0.0f, outerY),
        glm::vec2(outerX * 2.0f, thickness),
        kGold,
        3
    );

    drawMaterial(
        shader, quad,
        glm::vec2(0.0f, -outerY),
        glm::vec2(outerX * 2.0f, thickness),
        kGold,
        3
    );

    // Left and right.
    drawMaterial(
        shader, quad,
        glm::vec2(-outerX, 0.0f),
        glm::vec2(thickness, outerY * 2.0f),
        kGold,
        3
    );

    drawMaterial(
        shader, quad,
        glm::vec2(outerX, 0.0f),
        glm::vec2(thickness, outerY * 2.0f),
        kGold,
        3
    );

    // 6. Three playing zones.
    drawZone(shader, quad, zones_[2], false);
    drawZone(shader, quad, zones_[1], true);
    drawZone(shader, quad, zones_[0], false);

    drawCorner(
        shader, quad,
        glm::vec2(-0.825f, 0.625f),
        glm::radians(45.0f)
    );

    drawCorner(
        shader, quad,
        glm::vec2(0.825f, 0.625f),
        glm::radians(45.0f)
    );

    drawCorner(
        shader, quad,
        glm::vec2(-0.825f, -0.625f),
        glm::radians(45.0f)
    );

    drawCorner(
        shader, quad,
        glm::vec2(0.825f, -0.625f),
        glm::radians(45.0f)
    );
}
