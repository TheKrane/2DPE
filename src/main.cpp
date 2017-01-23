#include "Actor.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "ScriptingSystem.h"
#include "SceneManager.h"
#include "LogicSystem.h"
#include "LevelEditor.h"
#include <iostream>

#undef main

RenderSystem* g_pRenderSystem = nullptr;
InputSystem* g_pInputSystem = nullptr;
PhysicsSystem* g_pPhysicsSystem = nullptr;
ScriptingSystem* g_pScriptingSystem = nullptr;
LogicSystem* g_pLogicSystem = nullptr;
SceneManager* g_pSceneManager = nullptr;

int main()
{
	//Init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

	g_pRenderSystem = new RenderSystem(Vector2(1280, 720), Vector2(640, 360), 24, "2DPE Example");
	g_pInputSystem = new InputSystem();
	g_pPhysicsSystem = new PhysicsSystem();
	g_pScriptingSystem = new ScriptingSystem();
	g_pLogicSystem = new LogicSystem();
	g_pSceneManager = new SceneManager();

	//Scene manager
	g_pSceneManager->LoadScene("Data/TestScene.xml");
        
        //Level editor
        LevelEditor levelEditor;
        
	//Game loop
	bool quit = false;
	SDL_Event e;

	while (!quit)
	{
		g_pInputSystem->ProcessInput();

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
                                
                        levelEditor.OnEvent(e);
		}

                levelEditor.Update();

		//Update
                if(!g_pSceneManager->isPaused())
                {
                        g_pScriptingSystem->ProcessScripts();
                        g_pLogicSystem->ProcessLogicComponents();
                        g_pPhysicsSystem->ProcessPhysics();
                }

		//Render
		g_pRenderSystem->Clear();
		g_pRenderSystem->Render();
                levelEditor.DrawUI();
		g_pRenderSystem->Display();

	}

	//Shutdown
	delete g_pSceneManager;
	g_pSceneManager = nullptr;

	delete g_pScriptingSystem;
	g_pScriptingSystem = nullptr;

	delete g_pPhysicsSystem;
	g_pPhysicsSystem = nullptr;

	delete g_pInputSystem;
	g_pInputSystem = nullptr;

	delete g_pRenderSystem;
	g_pRenderSystem = nullptr;

	return 0;
}
