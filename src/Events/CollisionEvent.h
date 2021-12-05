#pragma once

#include "../ECS/EntityAndRegistry.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
    Entity a;
    Entity b;

    CollisionEvent(Entity a, Entity b) : a(a), b(b) {}
};