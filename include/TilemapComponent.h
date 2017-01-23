#ifndef TILEMAPCOMPONENT_H
#define TILEMAPCOMPONENT_H

#include "Component.h"
#include "GMath.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

class TilemapComponent : public Component
{
public:
	TilemapComponent();
	~TilemapComponent();

	bool Init(XMLElement* pData);
	void Register();

	void Update();

	int getTileAtPosition(Vector2 position);
	void setTileAtPosition(int x, int y, int tile) { if(x >= 0.0f && x < m_Width && y >= 0.0f && y < m_Height) m_TileMapArray[x][y] = tile; }
	void setCollisionTileAtPosition(int x, int y, int tile) { if(x >= 0.0f && x < m_Width && y >= 0.0f && y < m_Height) m_CollisionMapArray[x][y] = tile; }
	
	int getWidth() { return m_Width; }
	int getHeight() { return m_Height; }
        
        void resize(int width, int height);
private:
        SDL_Texture* m_pTexture;
        std::vector<std::vector<int>> m_CollisionMapArray;
        std::vector<std::vector<int>> m_TileMapArray;
        
        void UpdateTile(int x, int y);
        
        int m_Width, m_Height;
};

#endif
