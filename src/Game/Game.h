#pragma once

#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include "soloud.h"
#include "soloud_wav.h"

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"
#include "../Utils/CookyUtils.h"

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
    SoLoud::Wav gWave;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<EventBus> eventBus;
    std::unique_ptr<DebugDraw> debugDraw;
    std::unique_ptr<SoLoud::Soloud> gSoloud;

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
    void TestPhysicsSystem();

    static void SetFPS(int fps);

    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
    static int FPS;
    static int MILLISECS_PER_FRAME;
    static bool isPaused;
};