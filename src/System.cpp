#include "System.h"

System::System()
{
}

System::~System()
{
	while (!m_Components.empty()) delete m_Components.back(), m_Components.pop_back();
}

void System::RegisterComponent(Component * pComponent)
{
	m_Components.push_back(pComponent);
}
