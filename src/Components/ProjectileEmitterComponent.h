#pragma once

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

struct ProjectileEmitterComponent {
    glm::vec2 projectileVelocity;
    int repeatFrequency;
    int projectileDuration;
    int hitPercentDamage;
    bool isFriendly;
    Uint32 lastEmissionTime;

    ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFrequency = 0,
                               int projectileDuration = 10000, int hitPercentDamage = 10, bool isFriendly = false) {
        this->projectileVelocity = projectileVelocity;
        this->repeatFrequency = repeatFrequency;
        this->projectileDuration = projectileDuration;
        this->hitPercentDamage = hitPercentDamage;
        this->isFriendly = isFriendly;
        this->lastEmissionTime = SDL_GetTicks();
    }
};
