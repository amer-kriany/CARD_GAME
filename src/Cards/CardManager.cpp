
#include "CardManager.h"

#include <algorithm>
#include <iostream>
#include <numeric>

CardManager::CardManager()
    : randomEngine_(std::random_device{}())
{
    initializeDeck();
}

void CardManager::initializeDeck()
{
    deck_.clear();
    playedCards_.clear();

    // Create the player's five starting cards.
    for (int id = 1; id <= 5; ++id)
    {
        deck_.emplace_back(id);
    }

    std::cout << "CardManager initialized.\n";
    std::cout << "Player deck: " << deck_.size()
              << " cards\n";
}

bool CardManager::playRandomCard()
{
    // No cards left.
    if (deck_.empty())
    {
        std::cout << "No cards left in the deck!\n";
        return false;
    }

    // Select a random position in the remaining deck.
    std::uniform_int_distribution<std::size_t> distribution(
        0,
        deck_.size() - 1
    );

    const std::size_t randomIndex = distribution(randomEngine_);

    // Copy the selected card.
    Card selectedCard = deck_[randomIndex];

    // Remove it from the deck.
    deck_.erase(deck_.begin() + randomIndex);

    // Add it to the played cards.
    playedCards_.push_back(selectedCard);

    std::cout << "\nCard " << selectedCard.id()
              << " moved to the game field!\n";

    std::cout << "Cards remaining in deck: "
              << deck_.size() << '\n';

    std::cout << "Cards in game field: "
              << playedCards_.size() << '\n';

    return true;
}

const std::vector<Card>& CardManager::deck() const
{
    return deck_;
}

const std::vector<Card>& CardManager::playedCards() const
{
    return playedCards_;
}

int CardManager::deckCount() const
{
    return static_cast<int>(deck_.size());
}

int CardManager::playedCount() const
{
    return static_cast<int>(playedCards_.size());
}
