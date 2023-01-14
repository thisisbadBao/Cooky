#include "Game.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../System/MovementSystem.h"
#include "../System/RenderSystem.h"
#include "../System/AnimationSystem.h"
#include "../System/CollisionSystem.h"
#include "../System/RenderColliderSystem.h"

Game::Game() {
    isRunning = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    Logger::Log("Game constructor called!");
}

Game::~Game() {
    Logger::Log("Game destructor called!");
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL.");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 1600; // displayMode.w;
    windowHeight = 1200; // displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window) {
        Logger::Err("Error creating SDL window.");
        return;
    }

    renderer = SDL_CreateRenderer(
        window, 
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    isDebug = !isDebug;
                }
                break;
            }
    }
}

void Game::LoadLevel(int level) {
    // Add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();

    // Add assets
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "jungle-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");

    // Load the tilemap
    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

    for (int y = 0; y < mapNumRows; y++) {
            for (int x = 0; x < mapNumCols; x++) {
                char ch;
                mapFile.get(ch);
                int srcRectY = std::atoi(&ch) * tileSize;
                mapFile.get(ch);
                int srcRectX = std::atoi(&ch) * tileSize;
                mapFile.ignore();

                Entity tile = registry->CreateEntity();
                tile.AddComponent<TransformComponent>(glm::vec2(x * tileScale * tileSize, y * tileScale * tileSize), glm::vec2(tileScale, tileScale), 0.0);
                tile.AddComponent<SpriteComponent>("jungle-image", tileSize, tileSize, 0, srcRectX, srcRectY);
            }
    }
    mapFile.close();

    // Create entities
    Entity chopper = registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 30.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.AddComponent<AnimationComponent>(2, 15, true);

    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 120, 10.0), glm::vec2(1.5, 1.5), 0.0);
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);
    radar.AddComponent<AnimationComponent>(8, 9, true);

    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(700.0, 600.0), glm::vec2(2.0, 2.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(32, 32);

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(500.0, 600.0), glm::vec2(3.0, 3.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
    truck.AddComponent<BoxColliderComponent>(32, 32);
}

// Initialize game objects...
void Game::Setup() {
    LoadLevel(1);
}

// Update game object
void Game::Update() {
    // Wait until time has passed MILLISECS_PER_FRAME & fixing the FPS
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the current frame time
    millisecsPreviousFrame = SDL_GetTicks(); // millisecond

    // Invoke all the systems that need to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update();

    // Update registry
    registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 176, 202, 113, 255);
    SDL_RenderClear(renderer);

    // Invoke all the systems that need to render
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer);
    }
    SDL_RenderPresent(renderer); // Swap the buffer
}

void Game::Run() {
    Setup();
    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}