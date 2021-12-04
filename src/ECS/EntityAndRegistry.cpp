#include "EntityAndRegistry.h"
#include "../Logger/Logger.h"

int Entity::GetId() const {
    return id;
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

Entity Registry::CreateEntity() {
    int entityId = numEntities++;
    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }

    Logger::Log("Entity created with id:" + std::to_string(entityId));

    return entity;
}

void Registry::Update() {
    for (auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
}

void Registry::AddEntityToSystem(Entity entity) {
}
