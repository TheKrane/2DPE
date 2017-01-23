#include "TilemapComponent.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

extern RenderSystem* g_pRenderSystem;
extern PhysicsSystem* g_pPhysicsSystem;

TilemapComponent::TilemapComponent()
{
        m_Name = "TilemapComponent";
}

TilemapComponent::~TilemapComponent()
{
}

bool TilemapComponent::Init(XMLElement* pData)
{
        SDL_Surface* pSurface = IMG_Load(pData->Attribute("path"));

	if(pSurface == nullptr)
		printf("Could not load texture! SDL_Error: %s\n", SDL_GetError());

	m_pTexture = SDL_CreateTextureFromSurface(g_pRenderSystem->getRenderer() ,pSurface);

	SDL_FreeSurface(pSurface);

	if(m_pTexture == nullptr)
		printf("Could not load texture! SDL_Error: %s\n", SDL_GetError());
        
       int width = atoi(pData->Attribute("width"));
       int height = atoi(pData->Attribute("height"));
	
	//Load collisionmap
	m_CollisionMapArray.resize(width);
	for (int i = 0; i < width; i++)
		m_CollisionMapArray[i].resize(height);

	std::istringstream collisionMapSS(pData->FirstChildElement("Tilemap")->GetText());
	std::string collisionMapToken;

	int loaderCounterX = 0;
	int loaderCounterY = 0;

	while (std::getline(collisionMapSS, collisionMapToken, ' '))
	{
		int tile = atoi(collisionMapToken.c_str());
                
		m_CollisionMapArray[loaderCounterX][loaderCounterY] = tile;

		loaderCounterX++;
		if (loaderCounterX >= width)
		{
			loaderCounterX = 0;
			loaderCounterY++;
		}
	}
       
       //Load tilemap
	m_TileMapArray.resize(width);
	for (int i = 0; i < width; i++)
		m_TileMapArray[i].resize(height);
       
       m_Height = height;
       m_Width = width;
       
	return true;
}

void TilemapComponent::Register()
{
        g_pRenderSystem->RegisterComponent(this);
        g_pPhysicsSystem->RegisterTilemapComponent(this);
}

void TilemapComponent::Update()
{        
        //Draw
        for(unsigned int x = 0; x < m_TileMapArray.size(); x++)
        {
                for(int y = 0; y < (int)m_TileMapArray[x].size(); y++) //Unsigned int causes errors
                {
                        UpdateTile(x, y);
                        
                        if(m_TileMapArray[x][y] != 0)
                        {
                                float pixelsPerUnit = g_pRenderSystem->getPixelsPerUnit();
                                
                                SDL_Rect rect;
                                rect.x = x * pixelsPerUnit * (g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w) - g_pRenderSystem->getViewport().x;
                                rect.y = (-y* pixelsPerUnit) * 2 +  (g_pRenderSystem->getWindowSize().y) - 24.0f*2 - g_pRenderSystem->getViewport().y;
                                
                                rect.w = 24.0f * (g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w);
                                rect.h = 24.0f * (g_pRenderSystem->getWindowSize().y / g_pRenderSystem->getViewport().h);
								
								SDL_Rect tileRect;
								tileRect.x = ((m_TileMapArray[x][y] - 1) % 8) * 24 ;
								tileRect.y = ((m_TileMapArray[x][y] - 1) / 8) * 24;
								tileRect.w = 24;
								tileRect.h = 24;
								
                                //Draw
                                SDL_RenderCopy(g_pRenderSystem->getRenderer(), m_pTexture, &tileRect, &rect);
                        }
                }
        }
}

int TilemapComponent::getTileAtPosition(Vector2 position)
{        
		if(position.x >= 0.0f && position.x < m_Width && position.y >= 0.0f && position.y < m_Height)
			return m_CollisionMapArray[position.x][position.y];
		return 1;
}

void TilemapComponent::resize(int width, int height)
{
        m_CollisionMapArray.resize(width);
	for (int i = 0; i < width; i++)
		m_CollisionMapArray[i].resize(height);
                
        m_TileMapArray.resize(width);
	for (int i = 0; i < width; i++)
		m_TileMapArray[i].resize(height);
                
        m_Width = width;
        m_Height = height;
}

void TilemapComponent::UpdateTile(int x, int y)
{
        if(getTileAtPosition(Vector2(x, y)) == 1)
        {
			//From most dependencies to least
			//8
			if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 7;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 8;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 10;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 15;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 16;
			
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 25;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 26;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 27;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 28;
			
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 36;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 49;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 50;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 51;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 52;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 53;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 54;
			
			//6
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 2;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 9;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 11;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 18;
			
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 41;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 42;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 43;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 44;
			
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 45;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 46;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 47;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 48;
			
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 55;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 56;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 57;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 58;
				
			//5
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 1)
				m_TileMapArray[x][y] = 1;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 1 && getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 3;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 17;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 1 && getTileAtPosition(Vector2(x, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 19;
				
			else if(getTileAtPosition(Vector2(x-1, y+1)) == 0 && getTileAtPosition(Vector2(x, y+1)) == 1 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 37;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 &&
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x-1, y-1)) == 0 && getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 38;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 1 && getTileAtPosition(Vector2(x+1, y-1)) == 0)
				m_TileMapArray[x][y] = 39;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && getTileAtPosition(Vector2(x+1, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 40;
				
			//4
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 29;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 30;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 31;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 32;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 33;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 0 && 
				getTileAtPosition(Vector2(x-1, y)) == 1 && getTileAtPosition(Vector2(x+1, y)) == 1 &&
				getTileAtPosition(Vector2(x, y-1)) == 0)
				m_TileMapArray[x][y] = 34;
				
			else if(getTileAtPosition(Vector2(x, y+1)) == 1 && 
				getTileAtPosition(Vector2(x-1, y)) == 0 && getTileAtPosition(Vector2(x+1, y)) == 0 &&
				getTileAtPosition(Vector2(x, y-1)) == 1)
				m_TileMapArray[x][y] = 35;
        } 
}