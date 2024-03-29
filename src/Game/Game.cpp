#include "Game.h"

#include <iostream>
#include <memory>
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
#include <box2d/box2d.h>

#include "../Logger/Logger.h"
#include "../System/MovementSystem.h"
#include "../System/RenderSystem.h"
#include "../System/AnimationSystem.h"
#include "../System/CollisionSystem.h"
#include "../System/RenderColliderSystem.h"
#include "../System/DamageSystem.h"
#include "../System/KeyboardSystem.h"
#include "../System/CameraMovementSystem.h"
#include "../System/RenderTextSystem.h"
#include "../System/RenderGUISystem.h"
#include "../System/ScriptSystem.h"
#include "../System/PhysicsSystem.h"
#include "../System/CallbackEventSystem.h"

#include "../EventBus/Event.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;
int Game::FPS;
int Game::MILLISECS_PER_FRAME;
bool Game::isPaused;

Game::Game() {
    isRunning = false;
    isDebug = true;
    isPaused = false;
    registry = std::make_unique<Registry>();
    assetManager = std::make_unique<AssetManager>();
    eventBus = std::make_unique<EventBus>();
    debugDraw = std::make_unique<DebugDraw>();
    Logger::LogD("Game constructor called!");
}

Game::~Game() {
    Logger::LogD("Game destructor called!");
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL.");
        return;
    }
    if (TTF_Init() != 0) {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowWidth = 1200; // displayMode.w / 2;
    windowHeight = 900; // displayMode.h / 2;
    const char* windowTitle = "Cooky Engine";

    window = SDL_CreateWindow(
        windowTitle,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_RESIZABLE
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
    assetManager->SetRenderer(renderer);
    debugDraw->SetRenderer(renderer);

    // Init ImGui context
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

    // Init camera
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    // Init audio
    assetManager->InitSound();

    isRunning = true;
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        // ImGui SDL input
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();

        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        registry->GetSystem<KeyboardSystem>().HandleKeyEvent(sdlEvent, registry);

        // Handle core SDL event
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                if (sdlEvent.key.keysym.sym == SDLK_F1) {
                    isDebug = !isDebug;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (sdlEvent.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                        camera.w = windowWidth;
                        camera.h = windowHeight;
                        ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);
                        Logger::LogT("Window size changed!" + std::to_string(windowHeight) + " " + std::to_string(windowWidth));
                        break;
                }
                break;
        }
    }
}

// Initialize game objects...
void Game::Setup() {
    // Add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderGUISystem>();
    registry->AddSystem<ScriptSystem>();
    registry->AddSystem<PhysicsSystem>();
    registry->AddSystem<CallbackEventSystem>();

    // Subscribe event
    // registry->GetSystem<MovementSystem>().SubscribeToEvent(eventBus);
    // registry->GetSystem<DamageSystem>().SubscribeToEvent(eventBus);
    // registry->GetSystem<KeyboardSystem>().SubscribeToEvent(eventBus);
    registry->GetSystem<CallbackEventSystem>().Init(registry->entityEventBus);
    registry->GetSystem<CollisionSystem>().Init(registry->entityEventBus);

    // Create the Lua binding
    registry->GetSystem<ScriptSystem>().CreateLuaBindings(lua, registry, assetManager);
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package, sol::lib::math);

    assetManager->AddSoundStream("affection", "./assets/sounds/affection.wav");
    assetManager->AddSoundEffect("sel", "./assets/sounds/select.wav");
}

// Update game object
void Game::Update() {
    // Wait until time has passed MILLISECS_PER_FRAME & fixing the FPS
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the current frame time
    millisecsPreviousFrame = SDL_GetTicks(); // millisecond

    // Reset all event handlers
    // eventBus->Reset();

    if (isPaused) return;


    // Invoke all the systems that need to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<PhysicsSystem>().Update();
    registry->GetSystem<ScriptSystem>().Update();

    // Update registry
    registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 174, 208, 238, 255);
    SDL_RenderClear(renderer);

    // Invoke all the systems that need to render
    registry->GetSystem<RenderSystem>().Update(renderer, assetManager, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetManager, camera);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(debugDraw, camera);
        registry->GetSystem<RenderGUISystem>().Update(registry, camera, window, deltaTime, assetManager);
    }
    SDL_RenderPresent(renderer); // Swap the buffer

    registry->GetSystem<ScriptSystem>().UpdateScript(lua, registry, assetManager);
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    assetManager->DeinitSound();
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::SetFPS(int fps) {
    FPS = fps;
    MILLISECS_PER_FRAME = 1000 / fps;
}