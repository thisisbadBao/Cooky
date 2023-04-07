#pragma once

#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"

class Game {
private:
    bool isRunning;
    bool isDebug;
    int millisecsPreviousFrame = 0;
    double deltaTime;
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

    static void ReloadScript(sol::state& _lua, std::unique_ptr<Registry>& _registry, std::unique_ptr<AssetManager>& _assetManager, const std::string& scriptPath);
    static void SetFPS(int fps);

    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
    static int FPS;
    static int MILLISECS_PER_FRAME;
    static bool isPaused;
};