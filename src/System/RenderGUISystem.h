#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <SDL2/SDL.h>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../System/ScriptSystem.h"
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Game/ScriptLoader.h"

class RenderGUISystem: public System {
public:
    RenderGUISystem() = default;

    void Update(std::unique_ptr<Registry>& registry, SDL_Rect camera, SDL_Window* window, double dt, std::unique_ptr<SoLoud::Soloud>& gSoloud) {
        ImGui::NewFrame();
        if (ImGui::Begin("Debug Window")) {
            ImGui::Text("Set window size");
            int width = Game::windowWidth, height = Game::windowHeight;
            ImGui::Text("Window width: %d", width);
            ImGui::Text("Window height: %d", height);
            ImGui::Separator();
            if (ImGui::Button("Set Full Screen")) {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            if (ImGui::Button("Quit Full Screen")) {
                SDL_SetWindowFullscreen(window, 0);
            }
            if (ImGui::Button("Real Full Screen")) {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            }
            ImGui::Separator();
            static int newFPS = 60;
            ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.2f);
            ImGui::InputInt("input int", &newFPS);
            if (newFPS != Game::FPS && newFPS >= 10) {
                Game::SetFPS(newFPS);
            }
            ImGui::Text(
                "Mouse pos (x=%.1f, y=%.1f)",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y
            );
            ImGui::Separator();
            std::vector<std::string> scriptVec;
            ScriptLoader::GetScriptPath("./assets/scripts", scriptVec);
            for (auto path : scriptVec) {
                if (ImGui::Button(path.c_str())) {
                    registry->GetSystem<ScriptSystem>().AddScriptTobeReload("./assets/scripts/" + path);
                }
            }
            ImGui::Separator();
            if (ImGui::Button("Reset")) {
                registry->GetSystem<ScriptSystem>().ResetScript();
            }

            if (Game::isPaused) {
                if (ImGui::Button("Continue")) {
                    Game::isPaused = false;
                }
            } else {
                if (ImGui::Button("Pause")) {
                    Game::isPaused = true;
                }
            }

            static bool showImGuiDemo = false;
            ImGui::Checkbox("Show ImGui Demo Window", &showImGuiDemo);
            if (showImGuiDemo) {
                ImGui::ShowDemoWindow();
            }
            ImGui::PlotLines("Visualization", gSoloud->getWave(), 256);
            if (ImGui::Button("Play")) {
                SoLoud::Wav gWave;
                gWave.load("./assets/sounds/d_att.wav");
                gWave.setLooping(true);
                int res = gSoloud->play(gWave);
                Logger::Log(std::to_string(res));
            }
                
        }
        ImGui::End();

        // Display a small overlay window to display some basic info
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_Always, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.1f);

        if (ImGui::Begin("Basic Info", NULL, windowFlags)) {
            // FPS Info
            const int fps_interval = 10;
            static int fps_count = 0;
            static double fps_average = 0;
            static int fps_offset = 0;
            static double fps = 0.0;
            static float fps_values[fps_interval] = {0};

            if (fps_count % fps_interval == 0) {
                fps = fps_average / fps_interval;
                fps_values[fps_offset] = fps;
                fps_offset = (fps_offset + 1) % fps_interval;
                fps_average = 0;
            }
            fps_count++;
            fps_average += 1 / dt;

            ImGui::Text("FPS: %.2f", fps);
            
            float average = 0.0f;
            for (int i = 0; i < fps_interval; i++) {
                average += fps_values[i];
            }
            std::string overlay = "avg " + std::to_string(average / fps_interval);
            ImGui::PlotLines("FPS", fps_values, fps_interval, fps_offset, overlay.c_str(), 55.0f, 65.0f, ImVec2(0, 80.0f));

            // Entity Info
            ImGui::Text("Entity Count: %d", registry->GetEntityCount());
        }

        ImGui::End();
        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};