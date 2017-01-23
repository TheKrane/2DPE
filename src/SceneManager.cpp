#include "SceneManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "tinyxml2.h"

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "ScriptingSystem.h"
#include "TilemapComponent.h"
#include "PlayerController.h"
#include "CameraComponent.h"

using namespace tinyxml2;

extern RenderSystem* g_pRenderSystem;
extern PhysicsSystem* g_pPhysicsSystem;
extern ScriptingSystem* g_pScriptingSystem;

#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#define REGISTERCOMPONENT(n) m_ComponentFactory.Register<n>(#n);

SceneManager::SceneManager()
{
        REGISTERCOMPONENT(SpriteComponent);
        REGISTERCOMPONENT(PhysicsComponent);
        REGISTERCOMPONENT(ScriptComponent);
        REGISTERCOMPONENT(TilemapComponent);
        REGISTERCOMPONENT(PlayerController);
        REGISTERCOMPONENT(CameraComponent);
}

SceneManager::~SceneManager()
{
	while (!m_pActors.empty()) delete m_pActors.back(), m_pActors.pop_back();
}

bool SceneManager::LoadScene(const std::string& path)
{
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(path.c_str());
    XMLCheckResult(eResult);
    XMLNode* pRoot = doc.FirstChild();

	for (XMLElement* actorElement = pRoot->FirstChildElement("Actor"); actorElement; actorElement = actorElement->NextSiblingElement())
	{
		Actor* pActor = new Actor(actorElement->Attribute("name"), Vector2(std::stof(actorElement->Attribute("positionX")), std::stof(actorElement->Attribute("positionY"))), Vector2(1.0f, 1.0f));

		for (XMLElement* componentElement = actorElement->FirstChildElement(); componentElement; componentElement = componentElement->NextSiblingElement())
		{
			Component* pComponent = m_ComponentFactory.Create(componentElement->Name());
			pComponent->Register();
			pComponent->setOwner(pActor);
			pComponent->Init(componentElement);
			pActor->AddComponent(pComponent);
		}
		AddActor(pActor);
	}

	return true;
}
