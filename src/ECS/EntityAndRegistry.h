#ifndef REGISTRY_H
#define REGISTRY_H

#include <memory>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <deque>
#include "IPool.h"
#include "Pool.h"
#include "Signature.h"
#include "System.h"
#include "../Logger/Logger.h"

class Entity {
private:
    int id;
public:
    Entity(int id)
            : id(id) {};

    Entity(const Entity &entity) = default;

    int GetId() const;

    Entity &operator=(const Entity &other) = default;

    bool operator==(const Entity &other) const { return id == other.id; }

    bool operator!=(const Entity &other) const { return id != other.id; }

    bool operator>(const Entity &other) const { return id > other.id; }

    bool operator<(const Entity &other) const { return id < other.id; }

    void Kill();

    template<typename TComponent, typename ...TArgs>
    void addComponent(TArgs &&...args);

    template<typename TComponent>
    void removeComponent();

    template<typename TComponent>
    bool hasComponent() const;

    template<typename TComponent>
    TComponent &getComponent();

    class Registry *registry;
};

class Registry {
private:
    int numEntities = 0;

    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

    std::vector<std::shared_ptr<IPool>> componentPools;
    std::vector<Signature> entityComponentSignatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    std::deque<int> freeIds;

public:
    Registry();

    ~Registry();

    Entity CreateEntity();

    void KillEntity(Entity entity);

    void Update();

    template<typename TComponent, typename  ...TArgs>
    void AddComponent(Entity entity, TArgs &&...args);

    template<typename TComponent>
    bool HasComponent(Entity entity) const;

    template<typename TComponent>
    void RemoveComponent(Entity entity);

    template<typename TComponent>
    TComponent &GetComponent(Entity entity);

    template<typename TSystem, typename ...TArgs>
    void AddSystem(TArgs &&...args);

    template<typename TSystem>
    void RemoveSystem();

    template<typename TSystem>
    bool HasSystem() const;

    template<typename TSystem>
    TSystem &GetSystem() const;

    void AddEntityToSystems(Entity entity);

    void RemoveEntityFromSystems(Entity entity);
};


template<typename TComponent, typename... TArgs>
void Entity::addComponent(TArgs &&... args) {
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::removeComponent() {
    registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::hasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent &Entity::getComponent() {
    return registry->GetComponent<TComponent>(*this);
}

template<typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&... args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}


template<typename TSystem>
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem &Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent, typename  ...TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId]) {
        componentPools[componentId] = std::make_shared<Pool<TComponent>>();
    }

    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(
            componentPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Component Id " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
TComponent &Registry::GetComponent(Entity entity) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
}


#endif //REGISTRY_H
