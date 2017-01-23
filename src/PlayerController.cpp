#include "PlayerController.h"
#include "LogicSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "PhysicsComponent.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include <iostream>

extern LogicSystem* g_pLogicSystem;
extern InputSystem* g_pInputSystem;
extern RenderSystem* g_pRenderSystem;

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{

}

bool PlayerController::Init(XMLElement* pData)
{
    return true;
}

void PlayerController::Register()
{
    g_pLogicSystem->RegisterComponent(this);
}

void PlayerController::Update()
{
        PhysicsComponent* pPhysics = m_pOwner->getComponent<PhysicsComponent>("PhysicsComponent");
        SpriteComponent* pSprite = m_pOwner->getComponent<SpriteComponent>("SpriteComponent");
		
		Vector2 velocity = pPhysics->getSpeed();
		
                //Move
		if(pPhysics->isOnGround())
		{
			//Check if sprinting
			if(g_pInputSystem->getButton(1))
				m_MaxSpeed = 10.0f;
			else
				m_MaxSpeed = 5.0f;
			if((velocity.x > 0.0f && g_pInputSystem->getAxis(0)  <= 0.0f) || (velocity.x <0.0f && g_pInputSystem->getAxis(0)  >= 0.0f))
				velocity.x -= velocity.x * 0.25f;
			
			velocity.x += g_pInputSystem->getAxis(0) * 0.2f;
		}
		else
		{
			if((velocity.x > 0.0f && g_pInputSystem->getAxis(0)  <= 0.0f) || (velocity.x <0.0f && g_pInputSystem->getAxis(0)  >= 0.0f))
				velocity.x -= velocity.x * 0.05f;
			
			velocity.x += g_pInputSystem->getAxis(0) * 0.15f;
		}
		//Turn
		if(velocity.x < 0.0f)
			m_pOwner->setScale(Vector2(-1.0f, 1.0f));
		else if(velocity.x > 0.0f)
			m_pOwner->setScale(Vector2(1.0f, 1.0f));
		
		//Correct velocity
		if (velocity.x > m_MaxSpeed)
			velocity.x  = m_MaxSpeed;

		if (velocity.x < -m_MaxSpeed)
			velocity.x = -m_MaxSpeed;
		
        //Animate
        if(pPhysics->isOnGround())
                pSprite->SetAnimation(0);
        else
                pSprite->SetAnimation(1);
                
        //Jump
        if(g_pInputSystem->getButtonDown(0) && pPhysics->isOnGround())
        {
                velocity.y = 14.0f;
                pPhysics->setOnGround(false);
        }
		
		//Adjust gravity
		if(g_pInputSystem->getButton(0) && velocity.y > 0.0f)
			pPhysics->setGravityMultiplier(0.6f);
		else
			pPhysics->setGravityMultiplier(1.0f);
		
		//Set physics speed
		pPhysics->setSpeed(velocity);
}