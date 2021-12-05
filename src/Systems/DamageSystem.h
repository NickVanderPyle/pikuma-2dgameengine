#pragma once

#include "../ECS/System.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"

class DamageSystem : public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent &event) {
        Logger::Log("Collision between entites " + std::to_string(event.a.GetId()) + " and " +
                    std::to_string(event.b.GetId()));
        event.a.Kill();
        event.b.Kill();
    }

    void Update() {

    }
};