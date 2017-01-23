#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <memory>
#include <iostream>
#include "Actor.h"
#include <string>
#include "Utility.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool LoadScene(const std::string& path);
	void AddActor(Actor* pActor) { m_pActors.push_back(pActor); }

	template <class SubClass>
	void RegisterComponent(std::string id)
	{
		m_ComponentFactory.Register<SubClass>(id);
	}

	Actor* findActor(const std::string& name)
	{
		for (unsigned int i = 0; i < m_pActors.size(); i++)
		{
			if (m_pActors[i]->getName() == name)
			{
				return m_pActors[i];
			}
		}

		return nullptr;
	}

        bool isPaused() { return m_Pause; }
        void setPause(bool pause) { m_Pause = pause; }

private:
	std::vector<Actor*> m_pActors;
	GenericObjectFactory<Component, std::string> m_ComponentFactory;

	std::vector<Component*> m_LogicComponents;
        
        bool m_Pause;
};

#endif
