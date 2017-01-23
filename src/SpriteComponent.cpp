#include "SpriteComponent.h"
#include "Actor.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include <SDL_image.h>
#include <iostream>
#include <math.h>

extern RenderSystem* g_pRenderSystem;
extern SceneManager* g_pSceneManager;

SpriteComponent::SpriteComponent() : m_pTexture(nullptr)
{
	m_Name = "SpriteComponent";
	m_CurrentAnimation = 0;
}

SpriteComponent::~SpriteComponent()
{
	SDL_DestroyTexture(m_pTexture);
}

bool SpriteComponent::Init(XMLElement* pData)
{
	SDL_Surface* pSurface = IMG_Load(pData->Attribute("path"));

	if(pSurface == nullptr)
		printf("Could not load texture! SDL_Error: %s\n", SDL_GetError());

	m_pTexture = SDL_CreateTextureFromSurface(g_pRenderSystem->getRenderer() ,pSurface);

	SDL_FreeSurface(pSurface);

	if(m_pTexture == nullptr)
		printf("Could not load texture! SDL_Error: %s\n", SDL_GetError());

	std::cout << pData->Attribute("animations") << std::endl;

	if(std::stoi(pData->Attribute("animations")) == 0)
	{
		int width, height;
		SDL_QueryTexture(m_pTexture, nullptr, nullptr, &width, &height);

		m_CurrentFrame.x = 0;
		m_CurrentFrame.y = 0;
		m_CurrentFrame.w = width;
		m_CurrentFrame.h = height;
                
                std::cout << width << std::endl;
	}
        
	else
	{
                for(XMLElement* pAnimationElement = pData->FirstChildElement("animation"); pAnimationElement; pAnimationElement = pAnimationElement->NextSiblingElement("animation"))
                {
                        Animation animation(pAnimationElement->IntAttribute("cyclespersecond"));

                        for(XMLElement* pFrameElement = pAnimationElement->FirstChildElement("frame"); pFrameElement != nullptr; pFrameElement = pFrameElement->NextSiblingElement("frame"))
                        {
                                animation.AddFrame(pFrameElement->IntAttribute("x"), pFrameElement->IntAttribute("y"), pFrameElement->IntAttribute("w"), pFrameElement->IntAttribute("h"));
                        }
                        
                        m_Animations.push_back(animation);
                        m_CurrentFrame = animation.getCurrentFrame();
                }
	}

	return true;
}

void SpriteComponent::Register()
{
	g_pRenderSystem->RegisterComponent(this);
}

void SpriteComponent::Update()
{
        //Animate
        if(m_Animations.size() > 0 && !g_pSceneManager->isPaused())
        {
                m_Animations[m_CurrentAnimation].Play();
                m_CurrentFrame = m_Animations[m_CurrentAnimation].getCurrentFrame();
        }
        
	//Update transform
	float pixelsPerUnit = g_pRenderSystem->getPixelsPerUnit();

        int width, height;

        if(m_Animations.size() < 1)
        {
                SDL_QueryTexture(m_pTexture, nullptr, nullptr, &width, &height);
        }
        else
        {
                width = m_CurrentFrame.w;
                height = m_CurrentFrame.h;
        }

	SDL_Rect rect;
	rect.x = m_pOwner->getPosition().x * pixelsPerUnit * (g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w) - g_pRenderSystem->getViewport().x ;
	rect.y = (-m_pOwner->getPosition().y * pixelsPerUnit) * 2 +  (g_pRenderSystem->getWindowSize().y) - height*2 - g_pRenderSystem->getViewport().y;
        
	rect.w = width * (g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w) * m_pOwner->getScale().x;
	rect.h = height * (g_pRenderSystem->getWindowSize().y / g_pRenderSystem->getViewport().h)  * m_pOwner->getScale().y;

	//Determine flip direction 
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if(rect.w < 0 && rect.h < 0)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if(rect.w < 0)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL);
	else if(rect.h < 0)
		flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL);
	
	//Convert dimesnions to absolute values
	rect.w = abs(rect.w);
	rect.h = abs(rect.h);

	//Draw
	SDL_RenderCopyEx(g_pRenderSystem->getRenderer(), m_pTexture, &m_CurrentFrame, &rect, m_pOwner->getAngle(), nullptr, flip);
}