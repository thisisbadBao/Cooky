#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>

class Game {
    private:
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;

    public:
        Game();
        ~Game();
        void Initialize();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Run();
        void Destroy();

        int windowWidth;
        int windowHeight;
};

#endif