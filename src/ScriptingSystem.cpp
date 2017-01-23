#include "ScriptingSystem.h"
#include <string>
#include "Actor.h"

ScriptingSystem::ScriptingSystem()
{	
}

ScriptingSystem::~ScriptingSystem()
{
}

void ScriptingSystem::ProcessScripts()
{
	for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Update();
	}
}