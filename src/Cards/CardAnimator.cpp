
#include "CardAnimator.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

CardAnimator::CardAnimator()
{
}

glm::vec2 CardAnimator::deckPosition(int cardId) const
{
    // Original position of each card in the player's deck.
    float x = (cardId - 3) * 0.24f;

    return glm::vec2(x, -0.48f);
}

glm::vec2 CardAnimator::gamePosition(int index) const
{
    // Destination positions in the game field.
    float x = (index - 2) * 0.29f;

    return glm::vec2(x, 0.0f);
}

void CardAnimator::start(int cardId, int targetIndex)
{
    Animation animation;

    animation.cardId = cardId;

    animation.startPosition = deckPosition(cardId);
    animation.targetPosition = gamePosition(targetIndex);
    animation.currentPosition = animation.startPosition;

    animation.elapsedTime = 0.0f;
    animation.duration = ANIMATION_DURATION;
    animation.rotation = 0.0f;

    // Every card gets its own animation.
    animations_.push_back(animation);
}

void CardAnimator::update(float deltaTime)
{
    for (Animation& animation : animations_)
    {
        animation.elapsedTime += deltaTime;

        float t = animation.elapsedTime / animation.duration;

        // Clamp the animation progress between 0 and 1.
        t = std::min(t, 1.0f);

        // Smoothstep easing.
        float smoothT = t * t * (3.0f - 2.0f * t);

        // Move between the starting point and destination.
        glm::vec2 position = glm::mix(
            animation.startPosition,
            animation.targetPosition,
            smoothT
        );

        // Add a small upward arc during the movement.
        float arcHeight = 0.10f * std::sin(t * 3.14159265f);

        position.y += arcHeight;

        animation.currentPosition = position;

        // Rotate the card slightly during its movement.
        animation.rotation =
            0.22f * std::sin(t * 3.14159265f);
    }

    // Remove completed animations.
    // The cards will now be drawn as stationary cards.
    animations_.erase(
        std::remove_if(
            animations_.begin(),
            animations_.end(),
            [](const Animation& animation)
            {
                return animation.elapsedTime >= animation.duration;
            }
        ),
        animations_.end()
    );
}

bool CardAnimator::isCardAnimating(int cardId) const
{
    for (const Animation& animation : animations_)
    {
        if (animation.cardId == cardId)
            return true;
    }

    return false;
}

void CardAnimator::drawCard(
    Shader& shader,
    Quad& quad,
    const glm::vec2& position,
    float rotation
) const
{
    glm::mat4 model(1.0f);

    model = glm::translate(
        model,
        glm::vec3(position.x, position.y, 0.02f)
    );

    model = glm::rotate(
        model,
        rotation,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    model = glm::scale(
        model,
        glm::vec3(CARD_WIDTH, CARD_HEIGHT, 1.0f)
    );

    // Outer gold border.
    shader.setMat4("model", model);

    shader.setVec4(
        "tint",
        glm::vec4(0.78f, 0.55f, 0.20f, 1.0f)
    );

    shader.setInt("useTexture", 0);

    quad.draw();

    // Inner dark red card surface.
    model = glm::mat4(1.0f);

    model = glm::translate(
        model,
        glm::vec3(position.x, position.y, 0.03f)
    );

    model = glm::rotate(
        model,
        rotation,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    model = glm::scale(
        model,
        glm::vec3(
            CARD_WIDTH * 0.88f,
            CARD_HEIGHT * 0.82f,
            1.0f
        )
    );

    shader.setMat4("model", model);

    shader.setVec4(
        "tint",
        glm::vec4(0.16f, 0.045f, 0.055f, 1.0f)
    );

    quad.draw();

    // Small gold symbol in the center.
    model = glm::mat4(1.0f);

    model = glm::translate(
        model,
        glm::vec3(position.x, position.y, 0.04f)
    );

    model = glm::rotate(
        model,
        rotation,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    model = glm::scale(
        model,
        glm::vec3(0.035f, 0.035f, 1.0f)
    );

    shader.setMat4("model", model);

    shader.setVec4(
        "tint",
        glm::vec4(0.95f, 0.72f, 0.30f, 1.0f)
    );

    quad.draw();
}

void CardAnimator::draw(
    Shader& shader,
    Quad& quad,
    const CardManager& cardManager
) const
{
    shader.use();
    shader.setInt("useTexture", 0);

    // Draw cards remaining in the player's deck.
    for (const Card& card : cardManager.deck())
    {
        drawCard(
            shader,
            quad,
            deckPosition(card.id()),
            0.0f
        );
    }

    // Draw cards in the game field.
    const auto& playedCards = cardManager.playedCards();

    for (std::size_t i = 0; i < playedCards.size(); ++i)
    {
        const Card& card = playedCards[i];

        bool foundAnimation = false;

        // Check whether this card is currently moving.
        for (const Animation& animation : animations_)
        {
            if (animation.cardId == card.id())
            {
                drawCard(
                    shader,
                    quad,
                    animation.currentPosition,
                    animation.rotation
                );

                foundAnimation = true;
                break;
            }
        }

        // If it isn't moving, draw it at its final position.
        if (!foundAnimation)
        {
            drawCard(
                shader,
                quad,
                gamePosition(static_cast<int>(i)),
                0.0f
            );
        }
    }
}
