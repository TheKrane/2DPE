#include "PhysicsSystem.h"
#include <iostream>

PhysicsSystem::PhysicsSystem() 
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::RegisterComponent(PhysicsComponent* pComponent)
{
        m_Components.push_back(pComponent);
}

void PhysicsSystem::RegisterTilemapComponent(TilemapComponent* pComponent)
{
        m_pTilemapComponent = pComponent;
}

void PhysicsSystem::ProcessPhysics()
{
	//Check collisions
	for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		//Horizontal collision
                int bottomLeftX = int(m_Components[i]->getCollider().getRect().x + m_Components[i]->getSpeed().x/60.0f);
                int bottomLeftY = int(m_Components[i]->getCollider().getRect().y);
                int topRightX = int(m_Components[i]->getCollider().getRect().x + m_Components[i]->getCollider().getRect().w  + m_Components[i]->getSpeed().x/60.0f);
                int topRightY = int(m_Components[i]->getCollider().getRect().y + m_Components[i]->getCollider().getRect().h);
                
                for(int x = bottomLeftX; x <= topRightX; x++)
                {
                        for(int y = bottomLeftY; y <= topRightY; y++)
                        {
                                if(m_pTilemapComponent->getTileAtPosition(Vector2(x, y)))
                                        m_Components[i]->setSpeed(Vector2(0.0f, m_Components[i]->getSpeed().y));  
                        }
                }
                
		//Vertical bottom collision
                int leftX = int(m_Components[i]->getCollider().getRect().x + m_Components[i]->getSpeed().x/60.0f );
                int rightX = int(m_Components[i]->getCollider().getRect().x + m_Components[i]->getCollider().getRect().w + m_Components[i]->getSpeed().x/60.0f);
                
                bool onGround = false;
                
                for(int x = leftX; x <= rightX; x++)
                {
                         if(m_pTilemapComponent->getTileAtPosition(Vector2(x, int(m_Components[i]->getCollider().getRect().y + m_Components[i]->getSpeed().y/60.0f - 0.01f))))
                                onGround = true;
                }
                
                m_Components[i]->setOnGround(onGround);
                
		//Vertical top collision   
                for(int x = leftX; x <= rightX; x++)
                {
                         if(m_pTilemapComponent->getTileAtPosition(Vector2(x, int(m_Components[i]->getCollider().getRect().y + m_Components[i]->getCollider().getRect().h + m_Components[i]->getSpeed().y/60.0f))))
                                m_Components[i]->setSpeed(Vector2(m_Components[i]->getSpeed().x, 0.0f));
                }
		
		//Check collision with other objects
		for (unsigned int j = 0; j< m_Components.size(); j++)
                {
                        if(i != j)
                        {
                                if(m_Components[i]->getCollider().Overlaps(m_Components[j]->getCollider()))
                                {
                                        std::cout << "Overlaps" << std::endl;
                                }
                        }
                }
	}
	
	for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Update();
	}
}