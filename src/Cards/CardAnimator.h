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
    void update(float deltaTime);

    void draw(
        const Shader& shader,
        const Texture& texture,
        const Cards& cards,
        const CardManager& cardManager
    ) const;

private:
    struct Animation {
        int cardId;

        glm::vec2 startPosition;
        glm::vec2 targetPosition;
        glm::vec2 currentPosition;

        float elapsedTime;
        float duration;
        float rotation;
    };

    std::vector<Animation> animations_;

    static constexpr float ANIMATION_DURATION = 0.65f;
    static constexpr float SLOT_SPACING = 0.29f;
    static constexpr float CARD_SCALE = 1.3f;

    glm::vec2 deckPosition(int cardId) const;
    glm::vec2 gamePosition(int index) const;

    void drawCard(
        int cardId,
        const glm::vec2& position,
        float rotation,
        const Shader& shader,
        const Texture& texture,
        const Cards& cards
    ) const;
};