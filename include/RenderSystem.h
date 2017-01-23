#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include "System.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include <vector>
#include "Actor.h"

class RenderSystem : public System
{
public:
	RenderSystem(Vector2 windowSize, Vector2 viewportSize, int pixelsPerUnit, const std::string& title);
	~RenderSystem();

	void Clear();
	void Render();
	void Display();
        
	SDL_Renderer* getRenderer() { return m_pRenderer; }
	Vector2 getWindowSize();
	
	std::string getTitle() { return m_Title; }

	SDL_Window* getWindow() { return m_pWindow; }
	SDL_Rect getViewport() { return m_Viewport; }
	void setViewport(Rect rect) { m_Viewport.x = rect.x; m_Viewport.y = rect.y; m_Viewport.w = rect.w; m_Viewport.h = rect.h; };

	float getPixelsPerUnit() { return m_PixelsPerUnit; }

	void RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent);

private:
	SDL_Window*  m_pWindow;
	SDL_Renderer* m_pRenderer;

	std::string m_Title;

	SDL_Rect m_Viewport;
	float m_PixelsPerUnit;
        
	bool m_DrawPhysics;
	std::vector<PhysicsComponent*> m_PhysicsComponents;
};

#endif
