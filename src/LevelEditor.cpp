#include "LevelEditor.h"

#include <nanogui/formhelper.h>
#include <nanogui/window.h>

#include "SceneManager.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "CameraComponent.h"

extern SceneManager* g_pSceneManager;
extern RenderSystem* g_pRenderSystem;
extern InputSystem* g_pInputSystem;

LevelEditor::LevelEditor() : m_pTilemap(nullptr), m_pScreen(nullptr), m_pEditorWindow(nullptr)
{
        m_pTilemap = g_pSceneManager->findActor("Tilemap")->getComponent<TilemapComponent>("TilemapComponent");
        
        m_LevelWidth = m_pTilemap->getWidth();
        m_LevelHeight = m_pTilemap->getHeight();
        
        m_EditMode = true;
        
        m_pScreen = new nanogui::Screen(g_pRenderSystem->getWindow(), Eigen::Vector2i(g_pRenderSystem->getWindowSize().x, g_pRenderSystem->getWindowSize().y), g_pRenderSystem->getTitle());

        nanogui::FormHelper *gui = new nanogui::FormHelper(m_pScreen);
        m_pEditorWindow = gui->addWindow(Eigen::Vector2i(0, 0), "Level Editor");
        gui->addGroup("Dimensions                                                ");
        gui->addVariable("Width", m_LevelWidth);
        gui->addVariable("Height", m_LevelHeight);
        
        m_pScreen->setVisible(true);
        m_pScreen->performLayout();
        m_pEditorWindow->center();
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::OnEvent(SDL_Event event)
{
        m_pScreen->onEvent(event);
}

void LevelEditor::Update()
{
        if(g_pInputSystem->getButtonDown(3))
                m_EditMode = !m_EditMode;
        
        if(m_EditMode)
        {                
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                
                if(mouseX < m_pEditorWindow->position().x() || mouseX > m_pEditorWindow->position().x() + m_pEditorWindow->width() ||
                        mouseY < m_pEditorWindow->position().y() || mouseY > m_pEditorWindow->position().y() + m_pEditorWindow->height())
                {
                        mouseX = (mouseX + g_pRenderSystem->getViewport().x)/ (24.0f*(g_pRenderSystem->getWindowSize().x / g_pRenderSystem->getViewport().w));
                        mouseY = 15 - (mouseY + g_pRenderSystem->getViewport().y) / (24.0f*(g_pRenderSystem->getWindowSize().y / g_pRenderSystem->getViewport().h));
                        
                        if(mouseX <= m_pTilemap->getWidth() && mouseY <= m_pTilemap->getHeight())
                        {
                                if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
                                {
                                        m_pTilemap->setTileAtPosition(mouseX, mouseY, 13);
                                        m_pTilemap->setCollisionTileAtPosition(mouseX, mouseY, 1);
                                }
                                if((SDL_GetMouseState(NULL, NULL) & g_pInputSystem->getButton(2)) || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
                                {
                                        m_pTilemap->setTileAtPosition(mouseX, mouseY, 0);
                                        m_pTilemap->setCollisionTileAtPosition(mouseX, mouseY, 0);
                                }
                        }
                        
                        g_pSceneManager->findActor("Camera")->getComponent<CameraComponent>("CameraComponent")->setCameraMode(1);
                        g_pSceneManager->setPause(true);
                }
                
                m_pTilemap->resize(m_LevelWidth, m_LevelHeight);
        }
        
        else
        {
                g_pSceneManager->findActor("Camera")->getComponent<CameraComponent>("CameraComponent")->setCameraMode(0);
                g_pSceneManager->setPause(false);
        }
        
        m_pEditorWindow->setVisible(m_EditMode);

}

void LevelEditor::DrawUI()
{
        m_pScreen->drawAll();        
}