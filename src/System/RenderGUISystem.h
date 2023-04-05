#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <SDL2/SDL.h>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Game/ScriptLoader.h"

class RenderGUISystem: public System {
public:
    RenderGUISystem() = default;

    void Update(const std::unique_ptr<Registry>& registry, SDL_Rect camera, SDL_Window* window,
        sol::state& lua, std::unique_ptr<AssetManager>& assetManager) {
        ImGui::NewFrame();
        if (ImGui::Begin("Debug Window")) {
            ImGui::Text("Set window size");
            int width = Game::windowWidth, height = Game::windowHeight;
            ImGui::Text("Window width: %d", width);
            ImGui::Text("Window height: %d", height);
            if (ImGui::Button("Set Full Screen")) {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            if (ImGui::Button("Quit Full Screen")) {
                SDL_SetWindowFullscreen(window, 0);
            }
            if (ImGui::Button("Real Full Screen")) {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            }
        }
        std::vector<std::string> scriptVec;
        ScriptLoader::GetScriptPath("./assets/scripts", scriptVec);
        for (auto path : scriptVec) {
            if (ImGui::Button(path.c_str())) {
                Game::ReloadScript(lua, registry, assetManager, path);
            }
        }
        static bool showImGuiDemo = false;
        ImGui::Checkbox("Show ImGui Demo Window", &showImGuiDemo);
        if (showImGuiDemo) {
            ImGui::ShowDemoWindow();
        }
        ImGui::End();

        // Display a small overlay window to display the map position using the mouse
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.9f);
        if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
            ImGui::Text(
                "Mouse pos (x=%.1f, y=%.1f) FPS: %d",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y,
                Game::Get_Refresh_Rate()
            );
        }
        ImGui::End();
        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};