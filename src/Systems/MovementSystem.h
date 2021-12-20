#pragma once

#include "../ECS/EntityAndRegistry.h"
#include "../ECS/System.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Events/CollisionEvent.h"

class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
    }

    void onCollision(CollisionEvent &event) {
        Entity a = event.a;
        Entity b = event.b;
        Logger::Log("MovementSystem: Collision between entities " + std::to_string(a.GetId()) + " and " +
                    std::to_string(b.GetId()));

        if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
            OnEnemyHitsObstacle(a, b);
        }
        if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies")) {
            OnEnemyHitsObstacle(b, a);
        }
    }

    void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
        if (!enemy.hasComponent<RigidBodyComponent>() || !enemy.hasComponent<SpriteComponent>()) {
            return;
        }

        auto &rigidBody = enemy.getComponent<RigidBodyComponent>();
        auto &sprite = enemy.getComponent<SpriteComponent>();

        if (rigidBody.velocity.x != 0) {
            rigidBody.velocity.x *= -1;
            sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }
        if (rigidBody.velocity.y != 0) {
            rigidBody.velocity.y *= -1;
            sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
        }
    }

    void Update(double deltaTime) {
        for (auto entity: GetSystemEntities()) {
            auto &transform = entity.getComponent<TransformComponent>();
            const auto rigidBody = entity.getComponent<RigidBodyComponent>();

            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;

            const bool isEntityOutsideMap = (
                    transform.position.x < 0 ||
                    transform.position.x > Game::mapWidth ||
                    transform.position.y < 0 ||
                    transform.position.y > Game::mapHeight
            );

            if (isEntityOutsideMap && !entity.HasTag("player")) {
                entity.Kill();
            }
        }
    }
};
