#pragma once

#include "../ECS/System.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlledComponent.h"

class KeyboardControlSystem : public System {
public:
    KeyboardControlSystem() {
        RequireComponent<KeyboardControlledComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onCollision);
    }

    void onCollision(KeyPressedEvent &event) {
        for (auto entity: GetSystemEntities()) {
            const auto KeyboardControl = entity.getComponent<KeyboardControlledComponent>();
            auto &sprite = entity.getComponent<SpriteComponent>();
            auto &rigidBody = entity.getComponent<RigidBodyComponent>();

            switch (event.symbol) {
                case SDLK_UP:
                    rigidBody.velocity = KeyboardControl.upVelocity;
                    sprite.srcRect.y = sprite.height * 0;
                    break;
                case SDLK_RIGHT:
                    rigidBody.velocity = KeyboardControl.rightVelocity;
                    sprite.srcRect.y = sprite.height * 1;
                    break;
                case SDLK_DOWN:
                    rigidBody.velocity = KeyboardControl.downVelocity;
                    sprite.srcRect.y = sprite.height * 2;
                    break;
                case SDLK_LEFT:
                    rigidBody.velocity = KeyboardControl.leftVelocity;
                    sprite.srcRect.y = sprite.height * 3;
                    break;
            }
        }
    }

    void Update() {

    }
};