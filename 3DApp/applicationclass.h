#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_
#include "inputclass.h"
#include "d3dclass.h"
#include "shadermanagerclass.h"
#include "texturemanagerclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "zoneclass.h"
#include "cpuclass.h"
#include "soundclass.h"
#include "orthonWindowClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	ShaderManagerClass* m_ShaderManager;
	TextureManagerClass* m_TextureManager;
	OrthoWindowClass* m_FullScreenWindow;
	TimerClass* m_Timer;
	FpsClass* m_Fps;
	ZoneClass* m_Zone;
	CpuClass* m_Cpu;
	SoundClass* m_Sound;
};

#endif