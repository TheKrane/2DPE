#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "GMath.h"
#include <vector>
#include "Component.h"
#include <iostream>
#include <map>

class Animation
{
public:
	Animation(unsigned int cyclesPerSecond)
        {
                m_CurrentFrame = 0;
                m_FrameTime = 0;
                m_CyclesPerSecond = cyclesPerSecond;
        }
	~Animation() {}

	void AddFrame(float x, float y, float w, float h)
	{
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		m_Frames.push_back(rect);
	}

	SDL_Rect getCurrentFrame() { return m_Frames[m_CurrentFrame]; }
        
        void Play()
        {
                m_FrameTime++;
                if(m_FrameTime == 60/m_CyclesPerSecond)
                { 
                        m_CurrentFrame++;
                         if(m_CurrentFrame >= m_Frames.size())
                                m_CurrentFrame = 0;
                        m_FrameTime = 0;
                }
        }
        
        unsigned int m_CurrentFrame;
        unsigned int m_FrameTime;
        unsigned int m_CyclesPerSecond;

private:
	std::vector<SDL_Rect> m_Frames;
};

class SpriteComponent : public Component
{
public:
	SpriteComponent();
	~SpriteComponent();

	bool Init(XMLElement* pData);
	void Register();

	void Update();

        void SetAnimation(int animation) {m_CurrentAnimation = animation; }

private:
	SDL_Texture* m_pTexture;
	SDL_Rect m_CurrentFrame;

	std::vector<Animation> m_Animations;
	int m_CurrentAnimation;
};

#endif
