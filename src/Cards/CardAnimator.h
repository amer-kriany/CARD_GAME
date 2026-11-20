#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "CardManager.h"
#include "Cards.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

class CardAnimator {
public:
    CardAnimator();

    void start(int cardId, int targetIndex);

    // cameraMode: 1 = top-down (cards lie flat), 2 = player view (cards stand).
    void update(float deltaTime, int cameraMode);

    void draw(
        const Shader& shader,
        const Texture& texture,
        const Cards& cards,
        const CardManager& cardManager
    ) const;

private:
    struct Animation {
        int cardId;

        // Positions are in TABLE space: x right, y toward the enemy.
        glm::vec2 startPosition;
        glm::vec2 targetPosition;
        glm::vec2 currentPosition;

        float elapsedTime;
        float duration;

        float spin;    // radians, around the card's own normal
        float height;  // world units above the table (the arc of the throw)
    };

    std::vector<Animation> animations_;

    // 0 = flat on the table, 90 = standing up. Shared by every card.
    float standAngle_ = 0.0f;

    static constexpr float ANIMATION_DURATION = 0.65f;
    static constexpr float SLOT_SPACING = 0.29f;   // table units
    static constexpr float CARD_SCALE = 1.3f;      // extra size on top of the table scale
    static constexpr float ARC_HEIGHT = 0.35f;     // world units
    static constexpr float STAND_SPEED = 220.0f;   // degrees per second
    static constexpr float TABLE_CLEARANCE = 0.012f; // avoids z-fighting with the table

    glm::vec2 deckPosition(int cardId) const;
    glm::vec2 gamePosition(int index) const;

    // How far the card's geometry reaches along its local Z (in model units).
    // Needed so a standing card rests on the table instead of sinking into it.
    float halfDepth(int cardId) const;

    void drawCard(
        int cardId,
        const glm::vec2& position,
        float height,
        float spin,
        const Shader& shader,
        const Texture& texture,
        const Cards& cards
    ) const;
};