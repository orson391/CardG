#ifndef CARDG_H
#define CARDG_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>

// Forward declarations
struct Button;
struct Player;

struct Card {
    enum class Suit { Hearts, Diamonds, Clubs, Spades };
    enum class Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

    Suit suit;
    Rank rank;
    bool faceUp;

    Card(Suit s, Rank r, bool f = false);
    std::string toString() const;
};

std::vector<Card> initializeDeck();
std::vector<Card> randdom(std::vector<Card> myne);
std::vector<std::vector<Card>> divideIntoCombos(const std::vector<Card>& cards, int comboSize);
Card parseCard(const std::string& cardStr);

void broadcastMessage(const std::vector<Player>& players, const char* message);

void createServer(std::vector<Card> deck);
void joinClient();

void renderButtonWithText(SDL_Renderer* renderer, const Button& button, TTF_Font* font);

#endif // CARDG_H
