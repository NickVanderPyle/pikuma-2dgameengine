#include "Registry.h"
#include "../Logger/Logger.h"

void Registry::AddEntityToSystems(Entity entity)
{
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

Entity Registry::CreateEntity()
{
	int entityId = numEntities++;
	Entity entity(entityId);
	entitiesToBeAdded.insert(entity);

	Logger::Log("Entity created with id:" + std::to_string(entityId));

	return entity;
}

void Registry::Update()
{
}

void Registry::AddEntityToSystem(Entity entity)
{
}
