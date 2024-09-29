#include <SDL.h>
#include <iostream>
#include "GameManager.h"

const int TARGET_FPS = 5;
const int FRAME_DELAY = 1000 / TARGET_FPS;  // 16.67 ms per frame for 60 FPS

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Basic SDL App", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          1920, 1080, 
                                          SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer to draw
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop flag
    bool isRunning = true;
    SDL_Event event;

    int rows = 100;
    int cols = 100;

    // Dynamically allocate a 2D array
    bool** array = new bool*[rows];
    for (int i = 0; i < rows; ++i) {
        array[i] = new bool[cols];
    }

    // Seed the random number generator with the current time
    std::srand(std::time(0));

    // Fill the array with random true/false values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {

            array[i][j] = std::rand() % 2;  // Generates 0 or 1 (false or true)
        }
    }

    GameManager manager(array, rows, cols);

    // Main loop
    while (isRunning) {

        // Get the starting time of the frame
        Uint32 frameStart = SDL_GetTicks();

        // Clear the screen with a color (e.g., black)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Set to black
        SDL_RenderClear(renderer);

        manager.DrawMap(renderer);

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }

            if(event.type == SDL_MOUSEBUTTONDOWN) 
            {
                int x, y;
                Uint32 mouseState = SDL_GetMouseState(&x, &y);
                manager.CheckClickCollision(x, y, renderer);
            }
        }

        // Present the updated frame
        SDL_RenderPresent(renderer);

        manager.EnforceRules(renderer);

       // Get the time taken to render the frame
        Uint32 frameTime = SDL_GetTicks() - frameStart;

        // If the frame finished early, delay to cap the frame rate
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);  // Delay to match target frame rate
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
