
#pragma once

#include <vector>
#include <random>

#include "Card.h"

class CardManager {
public:
    CardManager();

    // Moves one random card from the deck to the game field.
    // Returns true if a card was moved.
    bool playRandomCard();

    // Read-only access to the card collections.
    const std::vector<Card>& deck() const;
    const std::vector<Card>& playedCards() const;

    int deckCount() const;
    int playedCount() const;

private:
    std::vector<Card> deck_;
    std::vector<Card> playedCards_;

    std::mt19937 randomEngine_;

    void initializeDeck();
};
