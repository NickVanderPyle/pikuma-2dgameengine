#pragma once

#include <SDL2/SDL.h>
#include "../ECS/System.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"

class RenderGuiSystem : public System {
public:
    RenderGuiSystem() = default;

    void Update(SDL_Window* window) {
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        if (ImGui::Begin("Spawn enemies")){
            ImGui::Text("Here is where we spawn new enemies...");

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }
};
