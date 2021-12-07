#pragma once

#include "../ECS/EntityAndRegistry.h"
#include "../ECS/System.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem : public System {
public:
    AnimationSystem() {
        RequireComponent<AnimationComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            auto &animation = entity.getComponent<AnimationComponent>();
            auto &sprite = entity.getComponent<SpriteComponent>();

            animation.currentFrame =
                    ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }
};
