#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Component.h"
#include "GMath.h"

class PlayerController : public Component
{
public:
    PlayerController();
    ~PlayerController();

    bool Init(XMLElement* pData);
    void Register();

    void Update();
    
private:
        Vector2 m_Velocity;
		float m_MaxSpeed;
};

#endif