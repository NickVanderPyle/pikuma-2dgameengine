#pragma once

#include "../Components/ProjectileComponent.h"
#include "../ECS/System.h"
#include "../ECS/EntityAndRegistry.h"

class ProjectileLifecycleSystem : public System {
public:
    ProjectileLifecycleSystem() {
        RequireComponent<ProjectileComponent>();
    }

    void Update() {
        for (auto entity: GetSystemEntities()) {
            auto projectile = entity.getComponent<ProjectileComponent>();

            if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
                entity.Kill();
            }
        }
    }
};