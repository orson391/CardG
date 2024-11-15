#ifndef PLAYERG_H
#define PLAYERG_H

#include <vector>
#include <string>
#include "CardG.h"

class Player {
private:
    std::string name;                  // Player's name
    std::vector<Card> hand;            // Player's hand (cards they hold)

public:
    // Constructor
    Player(const std::string& name);

    // Card management methods
    void addCard(const Card& card);
    void removeCard(const Card& card);
    void clearHand();

    // Accessor methods
    std::string getName() const;
    std::vector<Card> getHand() const;
    std::string showHand() const;

    // Mutator methods
    void setHand(const std::vector<Card>& newHand);

    // Utility functions
    bool hasCard(const Card& card) const;

    // Overload equality operator to compare two players
    bool operator==(const Player& other) const;
};

#endif // PLAYERG_H
