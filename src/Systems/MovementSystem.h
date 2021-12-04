#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/EntityAndRegistry.h"
#include "../ECS/System.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void Update(double deltaTime) {
        for (auto entity: GetSystemEntities()) {
            auto &transform = entity.getComponent<TransformComponent>();
            const auto rigidBody = entity.getComponent<RigidBodyComponent>();

            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;

            Logger::Log("Entity Id: " + std::to_string(entity.GetId())
                        + " position: ("
                        + std::to_string(transform.position.x)
                        + ", "
                        + std::to_string(transform.position.y)
                        + ")");
        }
    }
};

#endif //MOVEMENTSYSTEM_H
