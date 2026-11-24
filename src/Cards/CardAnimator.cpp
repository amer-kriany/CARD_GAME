#include "CardAnimator.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "WorldConfig.h"

CardAnimator::CardAnimator()
{
}

glm::vec2 CardAnimator::deckPosition(int cardId) const
{
    float x = ((cardId - 1) - 2) * SLOT_SPACING;

    return glm::vec2(x, -0.48f);
}

glm::vec2 CardAnimator::gamePosition(int index) const
{
    float x = (index - 2) * SLOT_SPACING;

    return glm::vec2(x, 0.0f);
}

float CardAnimator::halfDepth(int cardId) const
{
    return 0.10f + 0.01f * static_cast<float>(cardId - 1);
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

    animation.spin = 0.0f;
    animation.height = 0.0f;

    animations_.push_back(animation);
}

void CardAnimator::update(float deltaTime, int cameraMode)
{
    // ---- Flat <-> standing, driven by the camera mode ----
    float targetAngle = (cameraMode == 2) ? 90.0f : 0.0f;
    float step = STAND_SPEED * deltaTime;

    if (standAngle_ < targetAngle)
    {
        standAngle_ = std::min(standAngle_ + step, targetAngle);
    }
    else if (standAngle_ > targetAngle)
    {
        standAngle_ = std::max(standAngle_ - step, targetAngle);
    }

    // ---- Deck -> game field movement ----
    for (Animation& animation : animations_)
    {
        animation.elapsedTime += deltaTime;

        float t = animation.elapsedTime / animation.duration;
        t = std::min(t, 1.0f);

        // Smoothstep easing.
        float smoothT = t * t * (3.0f - 2.0f * t);

        animation.currentPosition = glm::mix(
            animation.startPosition,
            animation.targetPosition,
            smoothT
        );

       
        animation.height = ARC_HEIGHT * std::sin(t * 3.14159265f);

        animation.spin = 0.22f * std::sin(t * 3.14159265f);
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
    float height,
    float spin,
    const Shader& shader,
    const std::vector<std::unique_ptr<Texture>>& cardTextures,
    const Cards& cards
) const
{
    const float tableScale = world::TABLE_SCALE;
    const float cardScale = tableScale * CARD_SCALE;

    const float standRad = glm::radians(standAngle_);

    float standLift =
        halfDepth(cardId) * cardScale * std::sin(standRad);

    glm::mat4 model(1.0f);

   
    model = glm::translate(
        model,
        glm::vec3(
            position.x * tableScale,
            TABLE_CLEARANCE + height + standLift,
            -position.y * tableScale
        )
    );

    
    model = glm::rotate(
        model,
        standRad,
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    model = glm::rotate(
        model,
        spin,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    model = glm::scale(model, glm::vec3(cardScale));

    // Each card has its own image: card id 1 -> textures[0], 
    std::size_t index = static_cast<std::size_t>(cardId - 1);

    if (index >= cardTextures.size())
    {
        index = 0;
    }

    cards.drawCard(cardId, shader, *cardTextures[index], model);
}

void CardAnimator::draw(
    const Shader& shader,
    const std::vector<std::unique_ptr<Texture>>& cardTextures,
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
            0.0f,
            shader,
            cardTextures,
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
                    animation.height,
                    animation.spin,
                    shader,
                    cardTextures,
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
                0.0f,
                shader,
                cardTextures,
                cards
            );
        }
    }
}