#pragma once

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include "../ECS/System.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"
#include "../ECS/EntityAndRegistry.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"

class RenderGuiSystem : public System {
public:
    RenderGuiSystem() = default;

    void Update(SDL_Window *window, const std::unique_ptr<Registry> &registry, SDL_Rect &camera) {
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        if (ImGui::Begin("Spawn enemies",NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            static int posX = 0;
            static int posY = 0;
            static int scaleX = 1;
            static int scaleY = 1;
            static int velX = 0;
            static int velY = 0;
            static int health = 100;
            static float rotation = 0.0;
            static float projAngle = 0.0;
            static float projSpeed = 100.0;
            static int projRepeat = 10;
            static int projDuration = 10;
            const char *sprites[] = {"tank-image", "truck-image"};
            static int selectedSpriteIndex = 0;

            if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("position x", &posX);
                ImGui::InputInt("position y", &posY);
                ImGui::SliderInt("scale x", &scaleX, 1, 10);
                ImGui::SliderInt("scale y", &scaleY, 1, 10);
                ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Rigid Body", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("velocity x", &velX);
                ImGui::InputInt("velocity y", &velY);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
                ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
                ImGui::InputInt("repeat (sec)", &projRepeat);
                ImGui::InputInt("duration (sec)", &projDuration);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderInt("%", &health, 0, 100);
            }
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Create new enemy")) {
                Entity enemy = registry->CreateEntity();
                enemy.Group("enemies");
                enemy.addComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY),
                                                       glm::degrees(rotation));
                enemy.addComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                enemy.addComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 1);
                enemy.addComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
                double projVelX = cos(projAngle) * projSpeed;
                double projVelY = sin(projAngle) * projSpeed;
                enemy.addComponent<ProjectileEmitterComponent>(glm::vec2(projVelX, projVelY), projRepeat * 1000,
                                                               projDuration * 1000, 10, false);
                enemy.addComponent<HealthComponent>(health);

                posX = posY = velX = velY = selectedSpriteIndex = 0;
                scaleX = scaleY = 1;
                health = 100;
                rotation = projAngle = 0.0;
                projSpeed = 100.0;
                projRepeat = projDuration = 10;
            }
        }
        ImGui::End();

        ImGuiWindowFlags windowFlags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
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
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }
};
