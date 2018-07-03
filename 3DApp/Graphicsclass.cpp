#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{}

GraphicsClass::~GraphicsClass()
{}

bool GraphicsClass::Initialize(int screenHeight, int screenWidth, HWND hwnd)
{
	bool result;

	//Create the direct3d object
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	//initialize d3dobject
	result = m_Direct3D->Initialize(screenHeight, screenWidth, hwnd, VSYNC_ENABLED, FULL_SCREEN);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::ShutDown()
{
	//Release d3d object
	if (!m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	//Rdender d3d object
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render()
{
	bool result;
	//Use the d3d object to render the scene
	result = m_Direct3D->Render();
	if (!result)
	{
		return false;
	}
	return true;
}
