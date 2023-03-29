#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <SDL2/SDL.h>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../ECS/ECS.h"

class RenderGUISystem: public System {
public:
    RenderGUISystem() = default;

    void Update(const std::unique_ptr<Registry>& registry, SDL_Rect camera) {
        ImGui::NewFrame();
        if (ImGui::Begin("Debug Window")) {
            ImGui::Text("ImGui Test");
            static int posX = 0, posY = 42;
            ImGui::SliderInt("posX", &posX, 0, 1500);
            ImGui::SliderInt("posY", &posY, 0, 1500);
            if (ImGui::Button("Create new enenmy")) {
                Entity tank = registry->CreateEntity();
                tank.Group("enemy");
                tank.AddComponent<TransformComponent>(Vec2(posX, posY), Vec2(2.0, 2.0), 0.0);
                tank.AddComponent<RigidBodyComponent>(Vec2(100.0, 0.0));
                tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
                tank.AddComponent<BoxColliderComponent>(32, 32);
            }
        }
        ImGui::End();

        // Display a small overlay window to display the map position using the mouse
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.9f);
        if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
            ImGui::Text(
                "Map coordinates (x=%.1f, y=%.1f)",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y
            );
        }
        ImGui::End();
        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};