#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>

#include "../ECS/ECS.h"

class RenderGUISystem: public System {
public:
    RenderGUISystem() = default;

    void Update() {
        ImGui::NewFrame();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize;
        if (ImGui::Begin("Debug Window", NULL, windowFlags)) {
            ImGui::Text("ImGui Test");
            static int i1 = 0, i2 = 42;
            ImGui::SliderInt("slider int", &i1, -1, 3);
            ImGui::Button("OK", ImVec2(120, 0));
            static float angle = 0.0f;
            ImGui::SliderAngle("slider angle", &angle);
            ImGui::DragInt("drag int", &i2, 1);
        }

        ImGui::End();
        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
    }
};

#endif