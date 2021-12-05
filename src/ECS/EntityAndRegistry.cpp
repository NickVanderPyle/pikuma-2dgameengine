#include "EntityAndRegistry.h"

int Entity::GetId() const {
    return id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
}

Registry::Registry() {
    Logger::Log("Registry constructor called.");
}

Registry::~Registry() {
    Logger::Log("Registry destructor called.");
}

void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto &entityComponentSignature = entityComponentSignatures[entityId];

    for (auto &system: systems) {
        const auto &systemComponentSignature = system.second->GetComponentSignature();
        bool isSystemInterestedInComponent =
                (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isSystemInterestedInComponent) {
            system.second->AddEntityToSystem(entity);
        }
    }
}

void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto &system: systems) {
        system.second->RemoveEntityFromSystem(entity);
        const auto entityId = entity.GetId();
    }
}

Entity Registry::CreateEntity() {
    int entityId;
    if (freeIds.empty()) {
        entityId = numEntities++;
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id:" + std::to_string(entityId));

    return entity;
}

void Registry::KillEntity(Entity entity) {
    entitiesToBeKilled.insert(entity.GetId());
}

void Registry::Update() {
    for (auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);

        entityComponentSignatures[entity.GetId()].reset();

        freeIds.push_back(entity.GetId());
    }
    entitiesToBeKilled.clear();
}
