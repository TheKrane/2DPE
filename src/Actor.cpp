#include "Actor.h"
#include <iostream>

#include "Component.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ScriptComponent.h"
#include "TilemapComponent.h"

Actor::Actor(const std::string& name, Vector2 position, Vector2 scale) : m_Name(name), m_Position(position), m_Scale(scale)
{
}

Actor::~Actor()
{
}

void Actor::Update()
{
}

void Actor::AddComponent(Component* pComponent)
{
	m_Components.push_back(pComponent);
}

void Actor::setPosition(Vector2 position)
{
	m_Position = position;
}
