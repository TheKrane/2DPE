#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"
#include "GMath.h"
#include <vector>
#include <math.h>
#include <iostream>

class AABB
{
public:
        AABB() {}
        AABB(Rect rect) { m_Rect = rect; }
        ~AABB() {}
        
        bool Overlaps(AABB aabb)
        {                
                if(m_Rect.x < aabb.getRect().x + aabb.getRect().w && m_Rect.x + m_Rect.w > aabb.getRect().x &&
                        m_Rect.y < aabb.getRect().y + aabb.getRect().h && m_Rect.y + m_Rect.h > aabb.getRect().y)
                {
                        return true;
                }
                return false;
        }
        
        Rect getRect() { return m_Rect; }
        void setRect(Rect rect) { m_Rect = rect; }
        
        void setPosition(Vector2 position)
        {
                m_Rect.x = position.x;
                m_Rect.y = position.y;
        }
        
private:
        Rect m_Rect;
};

class PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	bool Init(XMLElement* pData);
	void Register();

	void Update();
        
        Vector2 getSpeed() { return m_Speed; }
        void setSpeed(Vector2 speed) {m_Speed = speed; }
        
        AABB getCollider() { return m_Collider; }
        
        bool isOnGround() { return m_OnGround; }
        void setOnGround(bool onGround) { m_OnGround = onGround; }
		
		Vector2 getPosition() { return m_Position; }
        void setPosition(Vector2 position) { m_Position = position; }
        
		void setGravityMultiplier(float gravityMultiplier) { m_GravityMultiplier = gravityMultiplier; }
		
private:
	AABB m_Collider;
	Vector2 m_ColliderOffset;
        
	bool m_isKinematic;
	
	Vector2 m_OldPosition;
	Vector2 m_Position;
	
	Vector2 m_OldSpeed;
	Vector2 m_Speed;
	
	bool m_WasOnGround;
	bool m_OnGround;
	
	float m_GravityMultiplier;
};

#endif
