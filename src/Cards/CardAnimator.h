
#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "CardManager.h"
#include "Quad.h"
#include "Shader.h"

class CardAnimator {
public:
    CardAnimator();

    void start(int cardId, int targetIndex);
    void update(float deltaTime);

    void draw(
        Shader& shader,
        Quad& quad,
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

    static constexpr float CARD_WIDTH = 0.19f;
    static constexpr float CARD_HEIGHT = 0.13f;
    static constexpr float ANIMATION_DURATION = 0.65f;

    glm::vec2 deckPosition(int cardId) const;
    glm::vec2 gamePosition(int index) const;

    bool isCardAnimating(int cardId) const;

    void drawCard(
        Shader& shader,
        Quad& quad,
        const glm::vec2& position,
        float rotation
    ) const;
};
