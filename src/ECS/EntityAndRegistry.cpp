#include "EntityAndRegistry.h"

int Entity::GetId() const {
    return id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
}

void Entity::Tag(const std::string &tag) {
    registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string &tag) const {
    return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string &group) {
    registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string &group) const {
    return registry->EntityBelongsToGroup(*this, group);
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

// Tag mgmt
void Registry::TagEntity(Entity entity, const std::string &tag) {
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string &tag) const {
    if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end()) {
        return false;
    }
    return entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string &tag) const {
    return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity) {
    auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end()) {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

// group mgmt
void Registry::GroupEntity(Entity entity, const std::string &group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string &group) const {
    auto groupEntities = entitiesPerGroup.at(group);
    return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string &group) const {
    auto &setOfEntities = entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity) {
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity == groupPerEntity.end()) {
        return;
    }

    groupPerEntity.erase(groupedEntity);

    auto group = entitiesPerGroup.find(groupedEntity->second);
    if (group == entitiesPerGroup.end()) {
        return;
    }

    auto entityInGroup = group->second.find(entity);
    if (entityInGroup == group->second.end()) {
        return;
    }

    group->second.erase(entityInGroup);
}