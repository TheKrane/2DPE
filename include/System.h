#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "Component.h"

class System
{
public:
	System();
	~System();

	void RegisterComponent(Component* pComponent);

protected:
	std::vector<Component*> m_Components;
};

#endif