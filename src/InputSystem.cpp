#include "InputSystem.h"
#include <LuaBridge/LuaBridge.h>
#include <lua.h>
#include <iostream>

InputSystem::InputSystem()
{
	m_Pad = SDL_GameControllerOpen(0);

	if (m_Pad)
    	printf("Found a valid controller, named: %s\n", SDL_GameControllerName(m_Pad));
		
	m_ButtonMap[0] = true;
}


InputSystem::~InputSystem()
{
	SDL_GameControllerClose(m_Pad);
}

void InputSystem::ProcessInput()
{
	m_PreviousButtonMap.clear();
	m_PreviousButtonMap.insert(m_ButtonMap.begin(), m_ButtonMap.end());
	m_ButtonMap.clear();
	
        m_HAxis = 0.0f;
        m_VAxis = 0.0f;
        
	if(m_Pad)
	{
                m_ButtonMap[0] = SDL_GameControllerGetButton(m_Pad, SDL_CONTROLLER_BUTTON_A);
                m_ButtonMap[1] = SDL_GameControllerGetButton(m_Pad, SDL_CONTROLLER_BUTTON_X);
                m_ButtonMap[2] = SDL_GameControllerGetButton(m_Pad, SDL_CONTROLLER_BUTTON_B);
                m_ButtonMap[3] = SDL_GameControllerGetButton(m_Pad, SDL_CONTROLLER_BUTTON_Y);
                
                if(SDL_GameControllerGetAxis(m_Pad, SDL_CONTROLLER_AXIS_LEFTX) < -8000 || SDL_GameControllerGetAxis(m_Pad, SDL_CONTROLLER_AXIS_LEFTX) > 8000)
                        m_HAxis = SDL_GameControllerGetAxis(m_Pad, SDL_CONTROLLER_AXIS_LEFTX)/32767.0f;
	}
        
        const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
        
        if (keystate[SDL_SCANCODE_UP])
                m_ButtonMap[0] = true;
	
        if (keystate[SDL_SCANCODE_SPACE])
                m_ButtonMap[1] = true;

        if (keystate[SDL_SCANCODE_LCTRL])
                m_ButtonMap[2] = true;
                
        if (keystate[SDL_SCANCODE_E])
                m_ButtonMap[3] = true;
        
        if(keystate[SDL_SCANCODE_RIGHT])
		m_HAxis = 1.0f;
	else if(keystate[SDL_SCANCODE_LEFT])
		m_HAxis = -1.0f;
                
        if(keystate[SDL_SCANCODE_UP])
		m_VAxis = 1.0f;
	else if(keystate[SDL_SCANCODE_DOWN])
		m_VAxis = -1.0f;
}


bool InputSystem::getButton(int button)
{
	return m_ButtonMap[button];
}

bool InputSystem::getButtonDown(int button)
{
	if(!m_PreviousButtonMap[button] && m_ButtonMap[button])
		return true;

	return false;
}

bool InputSystem::getButtonUp(int button)
{
	if(m_PreviousButtonMap[button] && !m_ButtonMap[button])
		return true;

	return false;
}

float InputSystem::getAxis(int axis)
{
        if(axis == 0)
                return m_HAxis;
                
        if(axis == 1)
                return m_VAxis;

	return 0.0f;
}