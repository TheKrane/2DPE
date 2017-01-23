#include "LogicSystem.h"

LogicSystem::LogicSystem()
{

}

LogicSystem::~LogicSystem()
{

}

void LogicSystem::ProcessLogicComponents()
{
    for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Update();
	}
}