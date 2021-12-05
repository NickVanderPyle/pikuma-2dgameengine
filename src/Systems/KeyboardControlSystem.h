#pragma once

#include "../ECS/System.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System {
public:
    KeyboardControlSystem() {

    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onCollision);
    }

    void onCollision(KeyPressedEvent &event) {
        std::string keyCode = std::to_string(event.symbol);
        std::string keySymbol(1, event.symbol);
        Logger::Log("KeyPressedEvent: keyCode:" + keyCode + " keySymbol:" + keySymbol);
    }

    void Update() {

    }
};