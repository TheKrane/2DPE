#include "PhysicsComponent.h"
#include "Actor.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "SceneManager.h"

extern PhysicsSystem* g_pPhysicsSystem;
extern RenderSystem* g_pRenderSystem;
extern SceneManager* g_pSceneManager;

PhysicsComponent::PhysicsComponent()
{
	m_Name = "PhysicsComponent";
}

PhysicsComponent::~PhysicsComponent()
{
	
}

bool PhysicsComponent::Init(XMLElement* pData)
{
	m_Position = m_pOwner->getPosition();
        
	XMLElement* pAABBElement = pData->FirstChildElement();
	m_Collider.setRect(Rect(pAABBElement->FloatAttribute("x") + m_pOwner->getPosition().x, pAABBElement->FloatAttribute("y")  + m_pOwner->getPosition().y,
                                        pAABBElement->FloatAttribute("w"),pAABBElement->FloatAttribute("h")));

	m_ColliderOffset.x = (m_pOwner->getScale().x - pAABBElement->FloatAttribute("w")) / + 2.0f + pAABBElement->FloatAttribute("x");
        m_ColliderOffset.y = pAABBElement->FloatAttribute("y");

	m_isKinematic = pData->BoolAttribute("kinematic");
	
	m_GravityMultiplier = 1.0f;
}

void PhysicsComponent::Register()
{
	g_pPhysicsSystem->RegisterComponent(this);
	g_pRenderSystem->RegisterPhysicsComponent(this);
}

void PhysicsComponent::Update()
{
	//Set old values
	m_OldPosition = m_Position;
	m_OldSpeed = m_Speed;
	m_WasOnGround = m_OnGround;
	
	//Gravity
	if(!m_isKinematic)
			m_Speed.y = m_OldSpeed.y - (40.0f*m_GravityMultiplier)/60.0f;
	
	if(m_OnGround)
	{
			m_Position.y = (int)m_Position.y;
			m_Speed.y = 0.0f;
	}
	
	m_Position += m_Speed/60.0f;
	
	m_pOwner->setPosition(m_Position);
	
	//Update collider
	m_Collider.setPosition(m_Position + m_ColliderOffset);
}