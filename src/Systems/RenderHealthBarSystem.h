#pragma once

#include <SDL2/SDL.h>
#include "../ECS/EntityAndRegistry.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/HealthComponent.h"
#include "../AssetStore/AssetStore.h"

class RenderHealthBarSystem : public System {
public:
    RenderHealthBarSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<HealthComponent>();
    }

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera) {
        for (auto entity: GetSystemEntities()) {
            const auto transform = entity.getComponent<TransformComponent>();
            const auto sprite = entity.getComponent<SpriteComponent>();
            const auto health = entity.getComponent<HealthComponent>();

            SDL_Color healthBarColor = {255, 255, 255};

            if (health.healthPercentage >= 0 && health.healthPercentage < 40) {
                healthBarColor = {255, 0, 0}; //red
            }
            if (health.healthPercentage >= 40 && health.healthPercentage < 80) {
                healthBarColor = {255, 255, 0}; //yellow
            }
            if (health.healthPercentage >= 80 && health.healthPercentage <= 100) {
                healthBarColor = {0, 255, 0};
            }

            int healthBarWidth = 15;
            int healthBarHeight = 3;
            double HealthBarPosX = (transform.position.x + (sprite.width * transform.scale.x)) - camera.x;
            double HealthBarPosY = transform.position.y - camera.y;

            SDL_Rect healthBarRect = {
                    static_cast<int>(HealthBarPosX),
                    static_cast<int>(HealthBarPosY),
                    static_cast<int>(healthBarWidth * (health.healthPercentage / 100.0)),
                    static_cast<int>(healthBarHeight)
            };
            SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
            SDL_RenderFillRect(renderer, &healthBarRect);

            std::string healthText = std::to_string(health.healthPercentage);
            SDL_Surface *surface = TTF_RenderText_Blended(assetStore->GetFont("pico8-font-5"), healthText.c_str(),
                                                          healthBarColor);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int labelWidth = 0;
            int labelHeight = 0;
            SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);
            SDL_Rect healthBarTextRectangle = {
                    static_cast<int>(HealthBarPosX),
                    static_cast<int>(HealthBarPosY),
                    labelWidth,
                    labelHeight
            };
            SDL_RenderCopy(renderer, texture, NULL, &healthBarTextRectangle);

            SDL_DestroyTexture(texture);
        }
    }
};
