#include "CardAnimator.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

CardAnimator::CardAnimator()
{
}

glm::vec2 CardAnimator::deckPosition(int cardId) const
{
    // Each card owns a fixed slot in the deck zone, based on its ID (1-5).
    // Fixed slots mean cards don't shift when another card is played.
    float x = ((cardId - 1) - 2) * SLOT_SPACING;

    return glm::vec2(x, -0.48f);
}

glm::vec2 CardAnimator::gamePosition(int index) const
{
    // Spread played cards across the central game area.
    float x = (index - 2) * SLOT_SPACING;

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

    animations_.push_back(animation);
}

void CardAnimator::update(float deltaTime)
{
    for (Animation& animation : animations_)
    {
        animation.elapsedTime += deltaTime;

        float t = animation.elapsedTime / animation.duration;
        t = std::min(t, 1.0f);

        // Smoothstep easing.
        float smoothT = t * t * (3.0f - 2.0f * t);

        glm::vec2 position = glm::mix(
            animation.startPosition,
            animation.targetPosition,
            smoothT
        );

        // Small upward arc.
        position.y += 0.10f * std::sin(t * 3.14159265f);

        animation.currentPosition = position;
        animation.rotation = 0.22f * std::sin(t * 3.14159265f);
    }

    // Remove finished animations.
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

void CardAnimator::drawCard(
    int cardId,
    const glm::vec2& position,
    float rotation,
    const Shader& shader,
    const Texture& texture,
    const Cards& cards
) const
{
    glm::mat4 model(1.0f);

    // Position the card in our current 2D table view.
    model = glm::translate(
        model,
        glm::vec3(position.x, position.y, 0.02f)
    );

    // Convert Wassim's XZ card plane into our current XY view.
    model = glm::rotate(
        model,
        glm::radians(-90.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Spin around the card's local vertical axis.
    model = glm::rotate(
        model,
        rotation,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Make the cards a bit bigger so they're readable.
    model = glm::scale(model, glm::vec3(CARD_SCALE));

    cards.drawCard(cardId, shader, texture, model);
}

void CardAnimator::draw(
    const Shader& shader,
    const Texture& texture,
    const Cards& cards,
    const CardManager& cardManager
) const
{
    shader.use();

    // 1. Cards still in the player's deck, each in its own slot.
    for (const Card& card : cardManager.deck())
    {
        drawCard(
            card.id(),
            deckPosition(card.id()),
            0.0f,
            shader,
            texture,
            cards
        );
    }

    // 2. Played cards (animating or settled).
    const auto& playedCards = cardManager.playedCards();

    for (std::size_t i = 0; i < playedCards.size(); ++i)
    {
        const Card& card = playedCards[i];

        bool isAnimating = false;

        for (const Animation& animation : animations_)
        {
            if (animation.cardId == card.id())
            {
                drawCard(
                    card.id(),
                    animation.currentPosition,
                    animation.rotation,
                    shader,
                    texture,
                    cards
                );

                isAnimating = true;
                break;
            }
        }

        if (!isAnimating)
        {
            drawCard(
                card.id(),
                gamePosition(static_cast<int>(i)),
                0.0f,
                shader,
                texture,
                cards
            );
        }
    }
}