#include "CameraComponent.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "InputSystem.h"

extern RenderSystem* g_pRenderSystem;
extern SceneManager* g_pSceneManager;
extern InputSystem* g_pInputSystem;

CameraComponent::CameraComponent() : m_pTarget(nullptr)
{
	m_Name = "CameraComponent";
}

CameraComponent::~CameraComponent()
{
}

bool CameraComponent::Init(XMLElement* pData)
{
	std::cout << "Initiliazing camera" << std::endl;
	
	m_CameraMode = CameraMode::SideScrolling;
        
        return true;
}

void CameraComponent::Register()
{
	g_pRenderSystem->RegisterComponent(this);
}

void CameraComponent::Update()
{
        if(m_pTarget == nullptr)
                m_pTarget = g_pSceneManager->findActor("Player");
                
	if(m_CameraMode == CameraMode::SideScrolling)
	{
		m_Position.x += ((m_pTarget->getPosition().x - g_pRenderSystem->getViewport().w/(24.0f * g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w) + 0.5f) - m_Position.x) * 0.3f;
		m_Position.y += ((m_pTarget->getPosition().y - g_pRenderSystem->getViewport().h/(24.0f * g_pRenderSystem->getWindowSize().y / g_pRenderSystem->getViewport().h) + 0.5f) - m_Position.y) * 0.3f;
        }
        
        if(m_CameraMode == CameraMode::Editing)
	{
                m_Position.x += g_pInputSystem->getAxis(0) * 0.2f;
                m_Position.y += g_pInputSystem->getAxis(1) * 0.2f;
        }
        
        m_pTilemap = g_pSceneManager->findActor("Tilemap")->getComponent<TilemapComponent>("TilemapComponent");
        
        if(m_pTilemap != nullptr)
        {
                 if(m_Position.x < 0.0f)
                                m_Position.x = 0.0f;
                if(m_Position.x > m_pTilemap->getWidth() - g_pRenderSystem->getWindowSize().x/48.0f)
                                m_Position.x = m_pTilemap->getWidth() - g_pRenderSystem->getWindowSize().x/48.0f;
                                
                if(m_Position.y < 0.0f)
                                m_Position.y = 0.0f;
                if(m_Position.y > m_pTilemap->getHeight() - g_pRenderSystem->getWindowSize().y/48.0f)
                                m_Position.y = m_pTilemap->getHeight() - g_pRenderSystem->getWindowSize().y/48.0f;
        }
                
        g_pRenderSystem->setViewport(Rect(m_Position.x*24.0f*(g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w) , -m_Position.y*24.0f*(g_pRenderSystem->getWindowSize().y / g_pRenderSystem->getViewport().h), g_pRenderSystem->getViewport().w, g_pRenderSystem->getViewport().h));
}