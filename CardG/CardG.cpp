// CardG.cpp: Defines the entry point for the application.

#include "CardG.h"

struct Button {
    SDL_Rect rect;
    SDL_Color color;
    std::string label;
};

struct Card {
    enum class Suit { Hearts, Diamonds, Clubs, Spades }; // Enum for suits
    enum class Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace }; // Enum for ranks

    Suit suit;     // The suit of the card
    Rank rank;     // The rank of the card
    bool faceUp;   // True if the card is face-up, false if face-down

    // Constructor for initializing the card
    Card(Suit s, Rank r, bool f = false) : suit(s), rank(r), faceUp(f) {}

    // Function to get a string representation of the card (for display or debugging)
    std::string toString() const {
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

        std::string suitStr;
        switch (suit) {
        case Suit::Hearts: suitStr = "Hearts"; break;
        case Suit::Diamonds: suitStr = "Diamonds"; break;
        case Suit::Clubs: suitStr = "Clubs"; break;
        case Suit::Spades: suitStr = "Spades"; break;
        }

        return rankStr + " of " + suitStr + (faceUp ? " (Face-up)" : " (Face-down)");
    }
};

std::vector<Card> initializeDeck() {
    std::vector<Card> deck;

    // Loop over all suits and ranks to create all 52 cards
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 2; rank <= 14; ++rank) {
            deck.push_back(Card(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank)));
        }
    }

    return deck;
}


struct Player {
    TCPsocket socket;                // Connection socket
    std::string name;                // Player name
    std::vector<Card> myCards;       // Cards in hand
    int score = 0;                   // Player's score (optional, for scoring games)
    bool isTurn = false;             // Whether it's the player's turn
    bool isConnected = true;         // Track if the player is connected
};


std::vector<Card> randdom(std::vector<Card> myne)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(myne.begin(), myne.end(), g);

    //for (const auto& mcard : myne) {
    //    std::cout << mcard.toString() << std::endl;
    //}
    return myne;
}

std::vector<std::vector<Card>> divideIntoCombos(const std::vector<Card>& cards, int comboSize) {
    std::vector<std::vector<Card>> combos;
    int totalNumbers = cards.size();

    for (int i = 0; i < totalNumbers; i += comboSize) {
        std::vector<Card> combo; // Corrected to store Card objects
        for (int j = i; j < i + comboSize && j < totalNumbers; ++j) {
            combo.push_back(cards[j]);
        }
        combos.push_back(combo);
    }

    return combos;
}


Card parseCard(const std::string& cardStr) {
    Card::Suit suit;
    Card::Rank rank;

    // Extract suit
    if (cardStr.find("Hearts") != std::string::npos) suit = Card::Suit::Hearts;
    else if (cardStr.find("Diamonds") != std::string::npos) suit = Card::Suit::Diamonds;
    else if (cardStr.find("Clubs") != std::string::npos) suit = Card::Suit::Clubs;
    else if (cardStr.find("Spades") != std::string::npos) suit = Card::Suit::Spades;

    // Extract rank
    if (cardStr.find("2") != std::string::npos) rank = Card::Rank::Two;
    else if (cardStr.find("3") != std::string::npos) rank = Card::Rank::Three;
    else if (cardStr.find("4") != std::string::npos) rank = Card::Rank::Four;
    else if (cardStr.find("5") != std::string::npos) rank = Card::Rank::Five;
    else if (cardStr.find("6") != std::string::npos) rank = Card::Rank::Six;
    else if (cardStr.find("7") != std::string::npos) rank = Card::Rank::Seven;
    else if (cardStr.find("8") != std::string::npos) rank = Card::Rank::Eight;
    else if (cardStr.find("9") != std::string::npos) rank = Card::Rank::Nine;
    else if (cardStr.find("10") != std::string::npos) rank = Card::Rank::Ten;
    else if (cardStr.find("Jack") != std::string::npos) rank = Card::Rank::Jack;
    else if (cardStr.find("Queen") != std::string::npos) rank = Card::Rank::Queen;
    else if (cardStr.find("King") != std::string::npos) rank = Card::Rank::King;
    else if (cardStr.find("Ace") != std::string::npos) rank = Card::Rank::Ace;

    return Card(suit, rank);
}




void broadcastMessage(const std::vector<Player>& players, const char* message) {
    for (const auto& player : players) {
        SDLNet_TCP_Send(player.socket, message, strlen(message) + 1);
    }
}


// Function to render a button with text
void renderButtonWithText(SDL_Renderer* renderer, const Button& button, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
    SDL_RenderFillRect(renderer, &button.rect);

    // Render button label
    SDL_Color textColor = { 255, 255, 255, 255 };  // White text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, button.label.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {
        button.rect.x + (button.rect.w - textWidth) / 2,
        button.rect.y + (button.rect.h - textHeight) / 2,
        textWidth, textHeight
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Server function
void createServer(std::vector<Card> deck) {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init Error: " << SDLNet_GetError() << std::endl;
        return;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, 12345) == -1) {
        std::cerr << "SDLNet_ResolveHost Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "SDLNet_TCP_Open Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return;
    }

    std::cout << "Cards Shuffled" << std::endl;
    std::vector<Card> RandCard = randdom(deck);

    std::cout << "Room created. Waiting for players on port 12345...\n";

    std::vector<Player> players;
    int currentPlayerIndex = 0;
    const int MAX_PLAYERS = 6;
    const int JOIN_TIME_LIMIT = 30;  // Time limit in seconds
    bool joiningClosed = false;  // Flag to track if joining period has ended
    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        // Get the current time and calculate the elapsed time
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<int> elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
        
        // Check if the time limit has passed and close joining if it's over
        int remainingTime = JOIN_TIME_LIMIT - elapsedTime.count();
        if (!joiningClosed) {
            if (remainingTime > 0) {
                std::cout << "Time remaining to join: " << remainingTime << " seconds.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            else if (!joiningClosed) {
                std::cout << "Joining time has ended. No more players can join.\n";
                joiningClosed = true;  // Mark that the join period has ended
            }
        }

        // Accept new connections while the joining period is open
        if (!joiningClosed) {
            TCPsocket client = SDLNet_TCP_Accept(server);
            if (client) {
                // Reject if joining time has ended
                if (elapsedTime.count() > JOIN_TIME_LIMIT) {
                    char rejectionMessage[] = "Sorry, the time to join has expired.";
                    SDLNet_TCP_Send(client, rejectionMessage, strlen(rejectionMessage) + 1);
                    SDLNet_TCP_Close(client);
                    std::cout << "Rejected a player, time to join has expired.\n";
                    continue;
                }

                // Accept the player if there is space
                if (players.size() < MAX_PLAYERS) {
                    char welcomeMessage[] = "Welcome to the game!";
                    SDLNet_TCP_Send(client, welcomeMessage, strlen(welcomeMessage) + 1);

                    // Add player to the list
                    players.push_back({ client, "Player " + std::to_string(players.size() + 1) });
                    std::cout << "Player joined. Total players: " << players.size() << "\n";

                    // Broadcast new player count
                    std::string playerCountMessage = "PLAYERS|" + std::to_string(players.size());
                    broadcastMessage(players, playerCountMessage.c_str());
                }
                else {
                    // Reject the player if the room is full
                    char rejectionMessage[] = "Sorry, the room is full. Please try again later.";
                    SDLNet_TCP_Send(client, rejectionMessage, strlen(rejectionMessage) + 1);
                    SDLNet_TCP_Close(client);
                    std::cout << "Rejected a player, room is full.\n";
                }
            }
        }

        // Once joining is closed, process player actions
        if (joiningClosed) {
            int pl = players.size();
            std::vector<std::vector<Card>> combos = divideIntoCombos(RandCard, pl);

            for (size_t i = 0; i < players.size(); ++i) {
                players[i].myCards = combos[i]; // Assign cards to each player

                // Serialize the cards
                std::string serializedCards;
                for (const Card& card : players[i].myCards) {
                    serializedCards += card.toString() + ";"; // Use ';' as a delimiter
                }

                // Send the serialized cards to the player
                int result = SDLNet_TCP_Send(players[i].socket, serializedCards.c_str(), serializedCards.size());
                if (result < serializedCards.size()) {
                    std::cerr << "Failed to send cards to player: " << players[i].name << "\n";
                }
                else {
                    std::cout << "Sent cards to " << players[i].name << ": " << serializedCards << "\n";
                }
            }
        }


    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
}

void joinClient() {
    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init Error: " << SDLNet_GetError() << std::endl;
        return;
    }

    std::string serverIP = "127.0.0.1"; // Change this to the host's IP
    int serverPort = 12345;

    // Resolve the host (server IP and port)
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, serverIP.c_str(), serverPort) == -1) {
        std::cerr << "SDLNet_ResolveHost Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return;
    }

    // Open the TCP connection
    TCPsocket client = SDLNet_TCP_Open(&ip);
    if (!client) {
        std::cerr << "SDLNet_TCP_Open Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return;
    }

    std::cout << "Connected to server at " << serverIP << ":" << serverPort << std::endl;

    char buffer[1024];
    bool running = true;

    while (running) {
        // Receive data from the server
        int bytesReceived = SDLNet_TCP_Recv(client, buffer, sizeof(buffer) - 1);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data
            std::string receivedData(buffer);
            std::vector<Card> receivedCards;

            // Parse the cards
            size_t pos = 0;
            while ((pos = receivedData.find(';')) != std::string::npos) {
                std::string cardStr = receivedData.substr(0, pos);
                receivedData.erase(0, pos + 1);

                // Convert the string back into a Card object
                Card card = parseCard(cardStr);
                receivedCards.push_back(card);
            }

            // Display the received cards
            std::cout << "Received cards:\n";
            for (const Card& card : receivedCards) {
                std::cout << card.toString() << "\n";
            }
        }
        else if (bytesReceived == 0) {
            // Server closed the connection
            std::cerr << "Connection closed by server." << std::endl;
            running = false;
            break;
        }
        else {
            // Handle errors
            std::cerr << "SDLNet_TCP_Recv Error: " << SDLNet_GetError() << std::endl;
            running = false;
            break;
        }

        // Send a simple acknowledgment to the server
        std::string action = "ACK";
        SDLNet_TCP_Send(client, action.c_str(), action.length() + 1);
    }

    // Cleanup
    SDLNet_TCP_Close(client);
    SDLNet_Quit();
    std::cout << "Disconnected from server." << std::endl;
}


bool isMouseOver(const SDL_Rect& rect, int mouseX, int mouseY) {
    return mouseX > rect.x && mouseX < rect.x + rect.w &&
        mouseY > rect.y && mouseY < rect.y + rect.h;
}

int main(int argc, char* argv[]) {

    std::vector<Card> deck = initializeDeck();
    std::cout << "Cards initalized" << std::endl;

    // Print all cards in the deck
    for (const auto& card : deck) {
        std::cout << card.toString() << std::endl;
    }
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "TTF Initialization failed: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL Button", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("D:\\Project\\CardG\\CardG\\arial.ttf", 16);
    // Make sure to have a font file in your directory
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // First button (black)
    Button button1 = { {200, 200, 100, 30}, {0, 0, 0, 255}, "Start Server" };

    // Second button (red)
    Button button2 = { {200, 250, 100, 30}, {255, 0, 0, 255}, "Exit" };

    Button button3 = { {200, 300, 100, 30}, {0, 0, 0, 255}, "Join Client" };

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            if (e.type == SDL_MOUSEMOTION) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Check for hover on button 1
                if (isMouseOver(button1.rect, mouseX, mouseY)) {
                    button1.color = { 0, 255, 0, 255 };  // Hover color (green)
                }
                else {
                    button1.color = { 0, 0, 0, 255 };  // Default color (black)
                }

                // Check for hover on button 2
                if (isMouseOver(button2.rect, mouseX, mouseY)) {
                    button2.color = { 0, 255, 0, 255 };  // Hover color (green)
                }
                else {
                    button2.color = { 255, 0, 0, 255 };  // Default color (red)
                }

                // Check for hover on button 3
                if (isMouseOver(button3.rect, mouseX, mouseY)) {
                    button3.color = { 0, 255, 0, 255 };  // Hover color (green)
                }
                else {
                    button3.color = { 0, 0, 0, 255 };  // Default color (black)
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Check for button 1 click
                if (isMouseOver(button1.rect, mouseX, mouseY)) {
                    std::cout << "Button 1 clicked! Starting server...\n";
                    createServer(deck);
                }

                // Check for button 2 click
                if (isMouseOver(button2.rect, mouseX, mouseY)) {
                    std::cout << "Button 2 clicked! Exiting...\n";
                    running = false;
                }
                if (isMouseOver(button3.rect, mouseX, mouseY)) {
                    std::cout << "Button 3 clicked! Joining...\n";
                    joinClient();
                    //running = false;
                }
            }
        }

        // Clear the screen with a light color (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set background color to white
        SDL_RenderClear(renderer);  // Clear screen with white background

        // Render both buttons
        renderButtonWithText(renderer, button1, font);
        renderButtonWithText(renderer, button2, font);
        renderButtonWithText(renderer, button3, font);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}