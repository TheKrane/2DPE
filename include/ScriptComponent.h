#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "Component.h"
#include <string>
#include <lua.hpp>

class ScriptComponent : public Component
{
public:
	ScriptComponent();
	~ScriptComponent();

	bool Init(XMLElement* pData);
	void Register();

	void Update();

private:
	lua_State* pState;
};

#endif
