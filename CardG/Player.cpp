#include "PlayerG.h"

Player::Player(const std::string& name) : name(name) {}

void Player::addCard(const Card& card) {
    hand.push_back(card);
}

void Player::removeCard(const Card& card) {
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        hand.erase(it);
    }
}

void Player::clearHand() {
    hand.clear();
}

std::string Player::getName() const {
    return name;
}

std::vector<Card> Player::getHand() const {
    return hand;
}

std::string Player::showHand() const {
    std::string result = name + "'s Hand:\n";
    for (const auto& card : hand) {
        result += "- " + card.toString() + "\n";
    }
    return result;
}

void Player::setHand(const std::vector<Card>& newHand) {
    hand = newHand;
}

bool Player::hasCard(const Card& card) const {
    return std::find(hand.begin(), hand.end(), card) != hand.end();
}

bool Player::operator==(const Player& other) const {
    return name == other.name && hand == other.hand;
}
