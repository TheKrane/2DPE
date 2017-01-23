#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <vector>
#include <memory>
#include "System.h"
#include "PhysicsComponent.h"
#include "TilemapComponent.h"
#include "Actor.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void ProcessPhysics();

        void RegisterComponent(PhysicsComponent* pComponent);
        void RegisterTilemapComponent(TilemapComponent* pComponent);

private:
        std::vector<PhysicsComponent*> m_Components;
        TilemapComponent* m_pTilemapComponent;
};

#endif