#include "Cards.h"

#include <stdexcept>

// Texture coordinates: v is flipped (1 at the first corner, 0 at the last).
// The first corners have z = -0.10, which is the FAR edge of the card
// (up on screen in camera 1, the top when standing in camera 2), and the
// image's top row is v = 1, so the image top must sit on that edge.
Cards::Cards()
    : card1(
        {
            {-0.07f, 0.002f, -0.10f},
            { 0.07f, 0.002f, -0.10f},
            { 0.07f, 0.002f,  0.10f},
            {-0.07f, 0.002f,  0.10f}
        },
        {
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        }
    ),

    card2(
        {
            {-0.06f, 0.004f, -0.09f},
            { 0.08f, 0.004f, -0.09f},
            { 0.08f, 0.004f,  0.11f},
            {-0.06f, 0.004f,  0.11f}
        },
        {
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        }
    ),

    card3(
        {
            {-0.05f, 0.006f, -0.08f},
            { 0.09f, 0.006f, -0.08f},
            { 0.09f, 0.006f,  0.12f},
            {-0.05f, 0.006f,  0.12f}
        },
        {
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        }
    ),

    card4(
        {
            {-0.04f, 0.008f, -0.07f},
            { 0.10f, 0.008f, -0.07f},
            { 0.10f, 0.008f,  0.13f},
            {-0.04f, 0.008f,  0.13f}
        },
        {
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        }
    ),

    card5(
        {
            {-0.03f, 0.010f, -0.06f},
            { 0.11f, 0.010f, -0.06f},
            { 0.11f, 0.010f,  0.14f},
            {-0.03f, 0.010f,  0.14f}
        },
        {
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        }
    )
{
}

const PolygonWithTexture& Cards::getCard(int cardId) const
{
    switch (cardId)
    {
        case 1: return card1;
        case 2: return card2;
        case 3: return card3;
        case 4: return card4;
        case 5: return card5;

        default:
            throw std::out_of_range("Invalid card ID");
    }
}

void Cards::draw(
    const Shader& shader,
    const Texture& texture,
    const glm::mat4& baseModel
) const
{
    card1.draw(shader, texture, baseModel);
    card2.draw(shader, texture, baseModel);
    card3.draw(shader, texture, baseModel);
    card4.draw(shader, texture, baseModel);
    card5.draw(shader, texture, baseModel);
}

void Cards::drawCard(
    int cardId,
    const Shader& shader,
    const Texture& texture,
    const glm::mat4& model
) const
{
    getCard(cardId).draw(shader, texture, model);
}