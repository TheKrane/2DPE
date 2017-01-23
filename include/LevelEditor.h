#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <nanogui/screen.h>
#include "TilemapComponent.h"

class LevelEditor
{
public:
        LevelEditor();
        ~LevelEditor();
        
        void OnEvent(SDL_Event event);
        
        void Update();
        void DrawUI();
        
private:
        TilemapComponent* m_pTilemap;
        
        bool m_EditMode;
        
        nanogui::Screen* m_pScreen;
        nanogui::Window* m_pEditorWindow;
        
        int m_LevelWidth, m_LevelHeight;
};

#endif // LEVELEDITOR_H