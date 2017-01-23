#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"
#include "Actor.h"
#include "TilemapComponent.h"
#include "GMath.h"

enum class CameraMode { SideScrolling, Editing };

class CameraComponent : public Component
{
public:
	CameraComponent();
	~CameraComponent();
	
	bool Init(XMLElement* pData);
	void Register();

	void Update();
        
        void setCameraMode(int cameraMode) { m_CameraMode = static_cast<CameraMode>(cameraMode); }
	
private:
	CameraMode m_CameraMode;
	Vector2 m_Position;
        
        Actor* m_pTarget;
        TilemapComponent* m_pTilemap;
};

#endif // CAMERACOMPONENT_H
