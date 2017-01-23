#include "RenderSystem.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>

RenderSystem::RenderSystem(Vector2 windowSize, Vector2 viewportSize, int pixelsPerUnit, const std::string& title)
{
	//Setup window and camera
	m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN);
	if(m_pWindow == nullptr)
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	else
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if(m_pRenderer == nullptr)
			printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_SetRenderDrawColor(m_pRenderer, 126, 192, 238, 255);
        
	//Set title
	m_Title = title;

	//Set viewport
	m_Viewport.x = 0.0f;
	m_Viewport.y = 0.0f;
	m_Viewport.w = viewportSize.x;
	m_Viewport.h = viewportSize.y;

	//Set pixels per unit
	m_PixelsPerUnit = (float)pixelsPerUnit;
        
	m_DrawPhysics = false;
}

RenderSystem::~RenderSystem()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	IMG_Quit();
	SDL_Quit();	
}

void RenderSystem::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, 126, 192, 238, 255);
	SDL_RenderClear(m_pRenderer);
}

void RenderSystem::Render()
{
	//Render
	for (unsigned int i = 0; i < m_Components.size(); i++)
		m_Components[i]->Update();
                
        if(m_DrawPhysics)
        {
                for (unsigned int i = 0; i < m_PhysicsComponents.size(); i++)
                {
                        Rect rect = m_PhysicsComponents[i]->getCollider().getRect();
                        
                        boxRGBA(m_pRenderer, rect.x * m_PixelsPerUnit * (getWindowSize().x/m_Viewport.w) - m_Viewport.x,
                                                                        rect.y *  m_PixelsPerUnit * -(getWindowSize().y/m_Viewport.h) + getWindowSize().y, 
                                                                        (rect.x + rect.w)  * m_PixelsPerUnit * (getWindowSize().x/m_Viewport.w) - m_Viewport.x,
                                                                        (rect.y + rect.h )  * m_PixelsPerUnit * -(getWindowSize().y/m_Viewport.h) + getWindowSize().y,
                                                                        255, 0, 0, 128);
                }
        }
}

void RenderSystem::Display()
{
	SDL_RenderPresent(m_pRenderer);
}

Vector2 RenderSystem::getWindowSize()
{
	int windowSizeX, windowSizeY;
	SDL_GetWindowSize(m_pWindow, &windowSizeX, &windowSizeY);
	return Vector2(windowSizeX, windowSizeY);
}

void RenderSystem::RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent)
{
        m_PhysicsComponents.push_back(pPhysicsComponent);
}
