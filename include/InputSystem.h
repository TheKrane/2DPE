#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SDL2/SDL.h>
#include <list>
#include <map>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void ProcessInput();

	bool getButton(int button);
	bool getButtonDown(int button);
	bool getButtonUp(int button);

	float getAxis(int axis);

private:
	SDL_GameController* m_Pad;

	std::map<int, bool> m_ButtonMap;
	std::map<int, bool> m_PreviousButtonMap;

	float m_HAxis;
        float m_VAxis;
};

#endif