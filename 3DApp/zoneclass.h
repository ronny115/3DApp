#ifndef _ZONECLASS_H_
#define _ZONECLASS_H_
#include "d3dclass.h"
#include "inputclass.h"
#include "shadermanagerclass.h"
#include "texturemanagerclass.h"
#include "timerclass.h"
#include "userinterfaceclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "positionclass.h"
#include "terrainclass.h"

class ZoneClass
{
public:
	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	bool Initialize(D3DClass*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float, int, int);

private:
	void HandleMovementInput(InputClass*, float);
	void LightMovement(float);
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);

	UserInterfaceClass* m_UserInterface;
	CameraClass* m_Camera;
	LightClass* m_Light;
	PositionClass* m_Position;
	TerrainClass* m_Terrain;
	bool m_displayUI, m_wireFrame;
	float m_frameTime;
	float angle;
};
#endif