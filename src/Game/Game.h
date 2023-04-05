#pragma once

#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
    bool isRunning;
    bool isDebug;
    int millisecsPreviousFrame = 0;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect camera;
    sol::state lua;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<EventBus> eventBus;

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

    static int Get_Refresh_Rate();

    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
};