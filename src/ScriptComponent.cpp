#include "ScriptComponent.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <LuaBridge/LuaBridge.h>

#include "InputSystem.h"
#include "GMath.h"
#include "SceneManager.h"
#include "Actor.h"
#include "PhysicsComponent.h"
#include "ScriptingSystem.h"

extern InputSystem* g_pInputSystem;
extern SceneManager* g_pSceneManager;
extern ScriptingSystem* g_pScriptingSystem;

void print_error(lua_State* state)
{
	const char* message = lua_tostring(state, -1);
	puts(message);
	lua_pop(state, 1);
}

InputSystem* GetInputSystem()
{
	return g_pInputSystem;
}

SceneManager* GetSceneManager()
{
	return g_pSceneManager;
}

ScriptComponent::ScriptComponent() : pState(nullptr)
{
	m_Name = "UnititializedScriptComponent";
}

ScriptComponent::~ScriptComponent()
{
}

bool ScriptComponent::Init(XMLElement* pData)
{
	m_Name = pData->Attribute("name");

	//Initialize lua state
	pState = luaL_newstate();
	luaL_openlibs(pState);

	int result = luaL_loadfile(pState, pData->Attribute("path"));

	if (result != LUA_OK)
		print_error(pState);

	result = lua_pcall(pState, 0, LUA_MULTRET, 0);

	if (result != LUA_OK)
		print_error(pState);

	//Add functions
	luabridge::getGlobalNamespace(pState).beginClass<InputSystem>("InputSystem")
		.addFunction("getAxis", &InputSystem::getAxis)
		.addFunction("getButton", &InputSystem::getButton)
		.addFunction("getButtonDown", &InputSystem::getButtonDown)
		.endClass();
	luabridge::getGlobalNamespace(pState).addFunction("GetInputSystem", GetInputSystem);

	luabridge::getGlobalNamespace(pState).beginClass<Vector2>("Vector2")
		.addConstructor<void(*) (const float&, const float&)>()
		.addData("x", &Vector2::x)
		.addData("y", &Vector2::y)
		.endClass();

	luabridge::getGlobalNamespace(pState).beginClass<Component>("Component")
		.addFunction("getName", &Component::getName)
		.addFunction("getOwner", &Component::getOwner)
		.endClass()
		.deriveClass<PhysicsComponent, Component>("PhysicsComponent")
		.endClass();

	luabridge::getGlobalNamespace(pState).beginClass<Actor>("Actor")
		.addFunction("getName", &Actor::getName)
		.addFunction("getPhysicsComponent", &Actor::getComponent<PhysicsComponent>)
		.addFunction("getScale", &Actor::getScale)
		.addFunction("setScale", &Actor::setScale)
		.endClass();


	luabridge::getGlobalNamespace(pState).beginClass<SceneManager>("SceneManager").addFunction("findActor", &SceneManager::findActor).endClass();
	luabridge::getGlobalNamespace(pState).addFunction("GetSceneManager", GetSceneManager);


	//Execute start function
	luabridge::getGlobal(pState, "Start")();

	return true;
}

void ScriptComponent::Register()
{
	g_pScriptingSystem->RegisterComponent(this);
}

void ScriptComponent::Update()
{
	luabridge::getGlobal(pState, "Update")();
}
