
#pragma once

#include "Graphics/PolygonWithTexture.h"

#include <glm/glm.hpp>

class Cards
{
public:
    Cards();

    void draw(
        const Shader& shader,
        const Texture& texture,
        const glm::mat4& baseModel
    ) const;

    void drawCard(
        int cardId,
        const Shader& shader,
        const Texture& texture,
        const glm::mat4& model
    ) const;

private:
    PolygonWithTexture card1;
    PolygonWithTexture card2;
    PolygonWithTexture card3;
    PolygonWithTexture card4;
    PolygonWithTexture card5;

    const PolygonWithTexture& getCard(int cardId) const;
};
