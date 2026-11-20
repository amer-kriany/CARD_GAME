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
    // Each card owns a fixed slot in the deck zone, based on its ID (1-5).
    float x = ((cardId - 1) - 2) * SLOT_SPACING;

    return glm::vec2(x, -0.48f);
}

glm::vec2 CardAnimator::gamePosition(int index) const
{
    // Spread played cards across the central game area.
    float x = (index - 2) * SLOT_SPACING;

    return glm::vec2(x, 0.0f);
}

float CardAnimator::halfDepth(int cardId) const
{
    // Wassim's cards: card 1 reaches z = 0.10, card 5 reaches z = 0.14.
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

        // The arc is now real height above the table (it used to be
        // a 2D "up the screen" shift).
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
    const Texture& texture,
    const Cards& cards
) const
{
    const float tableScale = world::TABLE_SCALE;
    const float cardScale = tableScale * CARD_SCALE;

    const float standRad = glm::radians(standAngle_);

    // When the card stands up it rotates around its CENTER, so half of it
    // would sink below the table. Lift it by how far it reaches downward.
    float standLift =
        halfDepth(cardId) * cardScale * std::sin(standRad);

    glm::mat4 model(1.0f);

    // Table space (x, y) -> world (x, up, z). The table's "toward the enemy"
    // axis y becomes -Z, so the deck (y = -0.48) is nearest the player.
    model = glm::translate(
        model,
        glm::vec3(
            position.x * tableScale,
            TABLE_CLEARANCE + height + standLift,
            -position.y * tableScale
        )
    );

    // Wassim's cards already lie on the XZ plane, so with this camera setup
    // they lie flat naturally. The old -90 degree hack is gone.
    // 0 degrees = flat, 90 degrees = standing, facing the player (+Z).
    model = glm::rotate(
        model,
        standRad,
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Spin around the card's own normal (works flat or standing).
    model = glm::rotate(
        model,
        spin,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    model = glm::scale(model, glm::vec3(cardScale));

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
                    animation.height,
                    animation.spin,
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
                0.0f,
                shader,
                texture,
                cards
            );
        }
    }
}