#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
    bool isRunning;
    int millisecsPreviousFrame = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;

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