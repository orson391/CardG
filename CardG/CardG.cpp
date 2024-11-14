// CardG.cpp : Defines the entry point for the application.
//

#include "CardG.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL Example",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer to draw on the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set the renderer draw color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // RGB color: blue

    // Clear the window with the draw color
    SDL_RenderClear(renderer);

    // Present the renderer to the window (updates the display)
    SDL_RenderPresent(renderer);

    // Wait for a few seconds
    SDL_Delay(3000); // 3 seconds

    // Clean up and close the SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;;
}
