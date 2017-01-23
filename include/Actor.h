#ifndef ACTOR_H
#define ACTOR_H

#include <vector>
#include <string>
#include <memory>
#include "GMath.h"
#include "Component.h"

class Actor
{
public:
	Actor(const std::string& name, Vector2 position, Vector2 scale);
	~Actor();

	void Update();

	void AddComponent(Component* pComponent);

	template <class ComponentType> ComponentType* getComponent(std::string componentName)
        {
                for (unsigned int i = 0; i < m_Components.size(); i++)
                {
                        if (m_Components[i]->getName() == componentName)
                        {
                                return (ComponentType*)m_Components[i];
                        }
                }

                return nullptr;
        }
        
	//Getters and Setters
	std::string getName() { return m_Name; }
	void setName(std::string name) { m_Name = name; }

	Vector2 getPosition() { return m_Position; }
	void setPosition(Vector2 position);

	float getAngle() { return m_Angle; }
	void setAngle(float angle) { m_Angle = angle; }

	Vector2 getScale() { return m_Scale; }
	void setScale(Vector2 scale) { m_Scale = scale; };


private:
	std::string m_Name;
	std::vector<Component*> m_Components;

	//Transform
	Vector2 m_Position;
	float m_Angle;
	Vector2 m_Scale;
};

#endif
