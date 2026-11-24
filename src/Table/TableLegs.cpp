#include "TableLegs.h"

#include <glm/gtc/matrix_transform.hpp>

#include "WorldConfig.h"

void TableLegs::drawBox(
    const Shader& shader,
    const glm::vec3& center,
    const glm::vec3& size
) const
{
   
    glm::mat4 model(1.0f);
    model = glm::translate(model, center);
    model = glm::scale(model, size);

    shader.setMat4("model", model);

    cube_.draw();
}

void TableLegs::draw(
    const Shader& shader,
    const Texture& woodTexture
) const
{
    shader.use();

   
    shader.setInt("useTexture", 1);
    shader.setInt("ourTexture", 0);
    shader.setVec4("tint", glm::vec4(0.60f, 0.40f, 0.25f, 1.0f));
    woodTexture.bind();

    const float halfWidth  = world::TABLE_WIDTH  * world::TABLE_SCALE * 0.5f;
    const float halfDepth  = world::TABLE_DEPTH  * world::TABLE_SCALE * 0.5f;

    // ---- Apron: a thick slab right under the table top ----
    const float apronHeight = 0.20f;
    const float apronTop = -0.01f;

    drawBox(
        shader,
        glm::vec3(0.0f, apronTop - apronHeight * 0.5f, 0.0f),
        glm::vec3(halfWidth * 2.0f, apronHeight, halfDepth * 2.0f)
    );

    // ---- Four legs,
    const float legThickness = 0.28f;
    const float inset = 0.30f;

    const float legTop = apronTop - apronHeight;
    const float legHeight = legTop - world::FLOOR_Y;

    const float legX = halfWidth - inset;
    const float legZ = halfDepth - inset;

    for (int sx = -1; sx <= 1; sx += 2)
    {
        for (int sz = -1; sz <= 1; sz += 2)
        {
            drawBox(
                shader,
                glm::vec3(
                    sx * legX,
                    world::FLOOR_Y + legHeight * 0.5f,
                    sz * legZ
                ),
                glm::vec3(legThickness, legHeight, legThickness)
            );
        }
    }
}
