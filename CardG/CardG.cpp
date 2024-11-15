﻿// CardG.cpp: Defines the entry point for the application.

#include "CardG.h"

struct Button {
    SDL_Rect rect;
    SDL_Color color;
    std::string label;
};

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
void createServer() {
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
        return;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, 12345) == -1) {
        fprintf(stderr, "SDLNet_ResolveHost Error: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return;
    }

    TCPsocket server = SDLNet_TCP_Open(&ip);
    if (!server) {
        fprintf(stderr, "SDLNet_TCP_Open Error: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return;
    }

    std::cout << "Server running on port 12345\n";
    bool running = true;

    while (running) {
        TCPsocket client = SDLNet_TCP_Accept(server);
        if (client) {
            std::cout << "Client connected!\n";
            const char* welcome = "Welcome to the server!\n";
            SDLNet_TCP_Send(client, welcome, strlen(welcome) + 1);
            SDLNet_TCP_Close(client);

            running = false;  // Exit after one connection for simplicity
        }
    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
}

bool isMouseOver(const SDL_Rect& rect, int mouseX, int mouseY) {
    return mouseX > rect.x && mouseX < rect.x + rect.w &&
        mouseY > rect.y && mouseY < rect.y + rect.h;
}

int main(int argc, char* argv[]) {
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
    Button button2 = { {200, 300, 100, 30}, {255, 0, 0, 255}, "Exit" };

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
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Check for button 1 click
                if (isMouseOver(button1.rect, mouseX, mouseY)) {
                    std::cout << "Button 1 clicked! Starting server...\n";
                    createServer();
                }

                // Check for button 2 click
                if (isMouseOver(button2.rect, mouseX, mouseY)) {
                    std::cout << "Button 2 clicked! Exiting...\n";
                    running = false;
                }
            }
        }

        // Clear the screen with a light color (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set background color to white
        SDL_RenderClear(renderer);  // Clear screen with white background

        // Render both buttons
        renderButtonWithText(renderer, button1, font);
        renderButtonWithText(renderer, button2, font);

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