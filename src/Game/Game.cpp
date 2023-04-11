#include "Game.h"

#include <iostream>
#include <memory>
#include <SDL2/SDL_image.h>
#include "../Utils/CookyUtils.h"
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
#include "../System/KeyboardControlSystem.h"
#include "../System/CameraMovementSystem.h"
#include "../System/RenderTextSystem.h"
#include "../System/RenderGUISystem.h"
#include "../System/ScriptSystem.h"
#include "../System/PhysicsSystem.h"
#include "../Game/ScriptLoader.h"

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

    // Init ImGui context
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

    // Init camera
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

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
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
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


void Game::TestPhysicsSystem() {
        for (int i = 0; i < 6; i++) {
            Entity ett = registry->CreateEntity();
            ett.AddComponent<RigidBodyComponent>();
            b2BodyDef body;
            body.type = b2_dynamicBody;
            body.position.Set(3, i);
            registry->GetSystem<PhysicsSystem>().AddPolygon(ett, body, 0.6, 0.6, 0.3f, 0.5f, 1);
            ett.AddComponent<TransformComponent>();
        }

        Entity plat = registry->CreateEntity();
        plat.AddComponent<RigidBodyComponent>();
        b2BodyDef platbody;
        platbody.type = b2_staticBody;
        platbody.position.Set(5, 9);
        registry->GetSystem<PhysicsSystem>().AddPolygon(plat, platbody, 8, 0.5, 0.3f, 0.5f, 1);
        plat.AddComponent<TransformComponent>();

        Entity poly = registry->CreateEntity();
        poly.AddComponent<RigidBodyComponent>();
        b2BodyDef polygon;
        polygon.position.Set(8, 6);
        polygon.type = b2_dynamicBody;
        polygon.linearVelocity.Set(-1, 0);
        b2Vec2 points[3] = {{2, 1}, {1, 2}, {3, 4}};
        registry->GetSystem<PhysicsSystem>().AddPolygon(poly, polygon, points, 3, 0.3f, 1, 1);
        poly.AddComponent<TransformComponent>();
}

// Initialize game objects...s
void Game::Setup() {
    // Add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderGUISystem>();
    registry->AddSystem<ScriptSystem>();
    registry->AddSystem<PhysicsSystem>();

    // Create the Lua binding
    registry->GetSystem<ScriptSystem>().CreateLuaBindings(lua, registry, assetManager);
    lua.open_libraries(sol::lib::base, sol::lib::math);

    TestPhysicsSystem();
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
    eventBus->Reset();

    if (isPaused) return;

    // Subcribe event
    registry->GetSystem<MovementSystem>().SubscribeToEvent(eventBus);
    registry->GetSystem<DamageSystem>().SubscribeToEvent(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvent(eventBus);

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
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
        registry->GetSystem<RenderGUISystem>().Update(registry, camera, window, deltaTime);
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
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::ReloadScript(sol::state& _lua, std::unique_ptr<Registry>& _registry,
    std::unique_ptr<AssetManager>& _assetManager, const std::string& scriptPath) {
    _registry->Reset();
    _registry->GetSystem<ScriptSystem>().ResetLuaState(_lua, _registry, _assetManager);
    ScriptLoader scriptLoader;
    scriptLoader.LoadScript(_lua, _registry, _assetManager, scriptPath);
}

void Game::SetFPS(int fps) {
    FPS = fps;
    MILLISECS_PER_FRAME = 1000 / fps;
}