#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "Entity.h"
#include "Signature.h"
#include "Component.h"

class System
{
private:
	Signature componentSignature;
	std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature &GetComponentSignature() const;

	template<typename TComponent> void RequireComponent();
};

template<typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}


#endif //SYSTEM_H
