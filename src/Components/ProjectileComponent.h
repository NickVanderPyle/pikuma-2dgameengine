#pragma once

#include <SDL2/SDL.h>

struct ProjectileComponent {
    bool isFriendly;
    int hitPercentDamage;
    int duration;
    Uint32 startTime;

    ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0, int duration = 0) {
        this->isFriendly = isFriendly;
        this->hitPercentDamage = hitPercentDamage;
        this->duration = duration;
        this->startTime = SDL_GetTicks();
    }
};