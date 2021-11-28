#ifndef REGISTRY_H
#define REGISTRY_H

#include <memory>
#include <set>
#include <unordered_map>
#include <typeindex>
#include "Entity.h"
#include "IPool.h"
#include "Pool.h"
#include "Signature.h"
#include "System.h"

class Registry
{
private:
	int numEntities = 0;

	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitiesToBeKilled;

	std::vector<std::shared_ptr<Pool<IPool>>> componentPools;
	std::vector<Signature> entityComponentSignatures;
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
	Registry();
    ~Registry();

	Entity CreateEntity();
	void Update();
	void AddEntityToSystem(Entity entity);

	template<typename TComponent, typename  ...TArgs> void AddComponent(Entity entity, TArgs &&...args);

	template<typename TComponent> bool HasComponent(Entity entity) const;
	template<typename TComponent> void RemoveComponent(Entity entity);

	template<typename TSystem, typename ...TArgs> void AddSystem(TArgs &&...args);
	template<typename TSystem> void RemoveSystem();
	template<typename TSystem> bool HasSystem() const;
	template<typename TSystem> TSystem &GetSystem() const;

	void AddEntityToSystems(Entity entity);
};

template<typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&... args)
{
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(typeid(TSystem), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem()
{
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const
{
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem &Registry::GetSystem() const
{
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent, typename  ...TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
        componentPools[componentId] = std::make_shared<Pool<TComponent>>();
	}

    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	if (entityId >= componentPool.GetSize()) {
		componentPool.Resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool.Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}


#endif //REGISTRY_H
