#pragma once

#include <SDL2/SDL.h>
#include "../Components/ProjectileEmitterComponent.h"
#include "../ECS/System.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityAndRegistry.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"

class ProjectileEmitSystem : public System {
public:
    ProjectileEmitSystem() {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::onKeyPressed);
    }

    void onKeyPressed(KeyPressedEvent &event) {
        if (event.symbol != SDLK_SPACE) {
            return;
        }
        for (auto entity: GetSystemEntities()) {
            if (!entity.hasComponent<CameraFollowComponent>()) {
                continue;
            }

            const auto projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
            const auto transform = entity.getComponent<TransformComponent>();
            const auto rigidBody = entity.getComponent<RigidBodyComponent>();

            glm::vec2 projectilePosition = transform.position;
            if (entity.hasComponent<SpriteComponent>()) {
                auto sprite = entity.getComponent<SpriteComponent>();
                projectilePosition.x += transform.scale.x * sprite.width / 2;
                projectilePosition.y += transform.scale.y * sprite.height / 2;
            }

            glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
            int directionX = 0;
            int directionY = 0;
            if (rigidBody.velocity.x > 0) directionX = +1;
            if (rigidBody.velocity.x < 0) directionX = -1;
            if (rigidBody.velocity.y > 0) directionY = +1;
            if (rigidBody.velocity.y < 0) directionY = -1;
            projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
            projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

            Entity projectile = entity.registry->CreateEntity();
            projectile.Group("projectiles");
            projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
            projectile.addComponent<RigidBodyComponent>(projectileVelocity);
            projectile.addComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
            projectile.addComponent<BoxColliderComponent>(4, 4);
            projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly,
                                                         projectileEmitter.hitPercentDamage,
                                                         projectileEmitter.projectileDuration);
        }
    }


    void Update(std::unique_ptr<Registry> &registry) {
        for (auto entity: GetSystemEntities()) {
            auto &projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
            const auto transform = entity.getComponent<TransformComponent>();

            if (projectileEmitter.repeatFrequency == 0) {
                continue;
            }

            if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
                glm::vec2 projectilePosition = transform.position;
                if (entity.hasComponent<SpriteComponent>()) {
                    const auto sprite = entity.getComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width / 2);
                    projectilePosition.y += (transform.scale.y * sprite.height / 2);
                }

                Entity projectile = registry->CreateEntity();
                projectile.Group("projectiles");
                projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                projectile.addComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                projectile.addComponent<SpriteComponent>("bullet-texture", 4, 4, 4);
                projectile.addComponent<BoxColliderComponent>(4, 4);
                projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly,
                                                             projectileEmitter.hitPercentDamage,
                                                             projectileEmitter.projectileDuration);

                projectileEmitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};