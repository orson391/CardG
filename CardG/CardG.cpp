#include "CardG.h"

Card::Card(Suit s, Rank r, bool f) : suit(s), rank(r), faceUp(f) {}

std::string Card::toString() const {
    // Convert rank to string
    std::string rankStr;
    switch (rank) {
    case Rank::Two: rankStr = "2"; break;
    case Rank::Three: rankStr = "3"; break;
    case Rank::Four: rankStr = "4"; break;
    case Rank::Five: rankStr = "5"; break;
    case Rank::Six: rankStr = "6"; break;
    case Rank::Seven: rankStr = "7"; break;
    case Rank::Eight: rankStr = "8"; break;
    case Rank::Nine: rankStr = "9"; break;
    case Rank::Ten: rankStr = "10"; break;
    case Rank::Jack: rankStr = "Jack"; break;
    case Rank::Queen: rankStr = "Queen"; break;
    case Rank::King: rankStr = "King"; break;
    case Rank::Ace: rankStr = "Ace"; break;
    }

    // Convert suit to string
    std::string suitStr;
    switch (suit) {
    case Suit::Hearts: suitStr = "Hearts"; break;
    case Suit::Diamonds: suitStr = "Diamonds"; break;
    case Suit::Clubs: suitStr = "Clubs"; break;
    case Suit::Spades: suitStr = "Spades"; break;
    }

    // Return the card as a string
    return rankStr + " of " + suitStr + (faceUp ? " (Face-up)" : " (Face-down)");
}

std::vector<Card> initializeDeck() {
    std::vector<Card> deck;

    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 2; rank <= 14; ++rank) {
            deck.emplace_back(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank));
        }
    }

    return deck;
}

std::vector<Card> randdom(std::vector<Card> myne) {
    // Shuffle the deck using a random number generator
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(myne.begin(), myne.end(), g);

    return myne;
}

std::vector<std::vector<Card>> divideIntoCombos(const std::vector<Card>& cards, int comboSize) {
    std::vector<std::vector<Card>> combos;

    // Divide cards into groups of comboSize
    for (size_t i = 0; i < cards.size(); i += comboSize) {
        std::vector<Card> combo;

        for (size_t j = i; j < i + comboSize && j < cards.size(); ++j) {
            combo.push_back(cards[j]);
        }

        combos.push_back(combo);
    }

    return combos;
}

Card parseCard(const std::string& cardStr) {
    // Parse the card string into a Card object
    size_t ofPos = cardStr.find(" of ");
    if (ofPos == std::string::npos) {
        throw std::invalid_argument("Invalid card string format.");
    }

    std::string rankStr = cardStr.substr(0, ofPos);
    std::string suitStr = cardStr.substr(ofPos + 4);

    // Convert rank string to Card::Rank
    Card::Rank rank;
    if (rankStr == "2") rank = Card::Rank::Two;
    else if (rankStr == "3") rank = Card::Rank::Three;
    else if (rankStr == "4") rank = Card::Rank::Four;
    else if (rankStr == "5") rank = Card::Rank::Five;
    else if (rankStr == "6") rank = Card::Rank::Six;
    else if (rankStr == "7") rank = Card::Rank::Seven;
    else if (rankStr == "8") rank = Card::Rank::Eight;
    else if (rankStr == "9") rank = Card::Rank::Nine;
    else if (rankStr == "10") rank = Card::Rank::Ten;
    else if (rankStr == "Jack") rank = Card::Rank::Jack;
    else if (rankStr == "Queen") rank = Card::Rank::Queen;
    else if (rankStr == "King") rank = Card::Rank::King;
    else if (rankStr == "Ace") rank = Card::Rank::Ace;
    else throw std::invalid_argument("Invalid rank string.");

    // Convert suit string to Card::Suit
    Card::Suit suit;
    if (suitStr == "Hearts") suit = Card::Suit::Hearts;
    else if (suitStr == "Diamonds") suit = Card::Suit::Diamonds;
    else if (suitStr == "Clubs") suit = Card::Suit::Clubs;
    else if (suitStr == "Spades") suit = Card::Suit::Spades;
    else throw std::invalid_argument("Invalid suit string.");

    return Card(suit, rank);
}
