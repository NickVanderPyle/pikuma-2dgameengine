#pragma once

#include "../ECS/System.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"

class DamageSystem : public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent &event) {
        Entity a = event.a;
        Entity b = event.b;
        Logger::Log("Collision between entites " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

        if (a.BelongsToGroup("projectiles") && b.HasTag("player")) {
            OnProjectileHitsPlayer(a, b);
        }
        if (b.BelongsToGroup("projectiles") && a.HasTag("player")) {
            OnProjectileHitsPlayer(b, a);
        }
        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) {
            OnProjectileHitsEnemy(a, b);
        }
        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) {
            OnProjectileHitsEnemy(b, a);
        }
    }

    void OnProjectileHitsPlayer(Entity projectile, Entity player) {
        auto projectileComponent = projectile.getComponent<ProjectileComponent>();

        if (projectileComponent.isFriendly) {
            return;
        }

        auto &health = player.getComponent<HealthComponent>();
        health.healthPercentage -= projectileComponent.hitPercentDamage;

        if (health.healthPercentage <= 0) {
            player.Kill();
        }

        projectile.Kill();
    }

    void OnProjectileHitsEnemy(Entity projectile, Entity enemy) {
        auto projectileComponent = projectile.getComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly) {
            return;
        }

        auto &health = enemy.getComponent<HealthComponent>();
        health.healthPercentage -= projectileComponent.hitPercentDamage;

        if (health.healthPercentage <= 0) {
            enemy.Kill();
        }

        projectile.Kill();
    }

    void Update() {

    }
};