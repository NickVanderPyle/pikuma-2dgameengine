#pragma once

#include "../ECS/EntityAndRegistry.h"
#include "../ECS/System.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem : public System {
public:
    CollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(std::unique_ptr<EventBus> &eventBus) {
        auto entities = GetSystemEntities();

        for (auto i = entities.begin(); i != entities.end(); i++) {
            auto a = *i;
            auto aTransform = a.getComponent<TransformComponent>();
            auto aCollider = a.getComponent<BoxColliderComponent>();

            for (auto j = i; j != entities.end(); j++) {
                auto b = *j;
                if (a == b) {
                    continue;
                }

                auto bTransform = b.getComponent<TransformComponent>();
                auto bCollider = b.getComponent<BoxColliderComponent>();

                auto collisionHappened = CheckAABBCollision(
                        aTransform.position.x + aCollider.offset.x,
                        aTransform.position.y + aCollider.offset.y,
                        aCollider.width,
                        aCollider.height,
                        bTransform.position.x + bCollider.offset.x,
                        bTransform.position.y + bCollider.offset.y,
                        bCollider.width,
                        bCollider.height);

                if (collisionHappened) {
                    Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with entity " +
                                std::to_string(b.GetId()));

                    eventBus->EmitEvent<CollisionEvent>(a, b);
                }
            }
        }
    }

    bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
        return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY < bY + bH &&
                aY + aH > bY
        );
    }
};
