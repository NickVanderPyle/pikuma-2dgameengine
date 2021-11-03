#ifndef COMPONENT_H
#define COMPONENT_H

#include "IComponent.h"

template<typename TComponent>
class Component: IComponent
{
	static int GetId()
	{
		static auto id = nextId++;
		return id;
	}
};

#endif //COMPONENT_H
