#pragma once

struct HealthComponent {
    int healthPercentage;

    HealthComponent(int healthPercentage = 0) {
        this->healthPercentage = healthPercentage;
    }
};