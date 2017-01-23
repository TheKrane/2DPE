#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <memory>
#include "tinyxml2.h"

using namespace tinyxml2;

class Actor;

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	virtual bool Init(XMLElement* pData) = 0;
	virtual void Register() = 0;

	virtual void Update() = 0;

	std::string getName() { return m_Name; }
	void setName(std::string name) { m_Name = name; }

	Actor* getOwner() { return m_pOwner; }
	void setOwner(Actor* pOwner) { m_pOwner = pOwner; }

protected:
	Actor* m_pOwner;
	std::string m_Name;
};

#endif
