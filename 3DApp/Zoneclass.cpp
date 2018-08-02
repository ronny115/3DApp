#define _USE_MATH_DEFINES
#include "zoneclass.h"

ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Light = 0;
	m_Position = 0;
	m_Terrain = 0;
	m_frameTime = 0;
	angle = 0;
	lerpPercent = 0;
	linearInterp = 0;
	sLinearInterp = 0;
	m_SkyDome = 0;
	m_DeferredBuffers = 0;
	m_LightShader = 0;
}

ZoneClass::ZoneClass(const ZoneClass& other)
{}

ZoneClass::~ZoneClass()
{}

bool ZoneClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	bool result;
	
	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if(!m_UserInterface)
	{
		return false;
	}
	result = m_UserInterface->Initialize(Direct3D, screenHeight, screenWidth);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}
	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	//Set the initial direction on the sunLight to calculate the radius for circular movement
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetSpecularColor(0.0f, 0.0f, 0.0f, 0.0f);
	//m_Light->SetSpecularPower(32000.0f);

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}
	// Set the initial position and rotation of the camera
	m_Position->SetPosition(512.0f, 100.0f, 512.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	m_DeferredBuffers = new DeferredBuffersClass;
	if (!m_DeferredBuffers)
	{
		return false;
	}
	result = m_DeferredBuffers->Initialize(Direct3D->GetDevice(), screenWidth, screenHeight, screenDepth, screenNear);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred buffer object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}
	result = m_Terrain->Initialize(Direct3D->GetDevice(),"data/setup.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	//create and initialize the skydome object
	m_SkyDome = new SkyDomeClass;
	if(!m_SkyDome)
	{
		return false;
	}
	result = m_SkyDome->Initialize(Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the skydome object.", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}
	result = m_LightShader->Initialize(Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the UI to display by default.
	m_displayUI = true;

	//set wire frame rendering to enabled
	m_wireFrame = false;

	linearInterp = new Vector3;
	if (!linearInterp)
	{
		return false;
	}

	sLinearInterp = new Vector2;
	if (!linearInterp)
	{
		return false;
	}
	return true;
}

void ZoneClass::Shutdown()
{
	// Release the skydome object.

	if (m_DeferredBuffers)
	{
		m_DeferredBuffers->Shutdown();
		delete m_DeferredBuffers;
		m_DeferredBuffers = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}


	// Release the user interface object.
	if(m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = 0;
	}

	return;
}

bool ZoneClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, OrthoWindowClass* FullScreenWindow, float frameTime, int fps, int cpu)
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Do the frame input processing.
	HandleMovementInput(Input, frameTime);
	//Move the light
	LightMovement(frameTime);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Do the frame processing for the user interface.
	result = m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, cpu, posX, posY, posZ, rotX, rotY, rotZ);
	if(!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render(Direct3D, ShaderManager, TextureManager, FullScreenWindow);
	if(!result)
	{
		return false;
	}

	return true;
}

void ZoneClass::HandleMovementInput(InputClass* Input, float frameTime)
{
	bool keyDown;
	int mouseX,mouseY;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	//Mouse movement to control the camera
	mouseX = Input->GetMouseX(mouseX);
	mouseY = Input->GetMouseY(mouseY);
	m_Position->MouseMovement(mouseX, mouseY);

	keyDown = Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = Input->IsUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = Input->IsDownPressed();
	m_Position->LookDownward(keyDown);
	//WASD movement
	keyDown = Input->IsWPressed();
	m_Position->MoveForward(keyDown);

	keyDown = Input->IsSPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = Input->IsAPressed();
	m_Position->MoveLeft(keyDown);

	keyDown = Input->IsDPressed();
	m_Position->MoveRight(keyDown);
	
	//Up & Down
	keyDown = Input->IsSpacePressed();
	m_Position->MoveUpward(keyDown);

	keyDown = Input->IsControlPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = Input->IsPgUpPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = Input->IsPgDownPressed();
	m_Position->MoveDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Determine if the user interface should be displayed or not.
	if(Input->IsF1Toggled())
	{
		m_displayUI = !m_displayUI;
	}

	//determine if the terrain should be rendered in wireframe or not
	if (Input->IsF2Toggled())
	{
		m_wireFrame = !m_wireFrame;
	}

	return;
}

void ZoneClass::LightMovement(float frameTime)
{
	float dX, dZ, radius, angleIncremet;
	XMFLOAT3 lightDirection;
	//Sky dome gradients colors
	Vector3 dawnApexColor = { 0.094f ,0.015f ,0.435f };
	Vector3 dawnCenterColor = { 0.866f ,0.439f ,0.019f };
	Vector3 noonApexColor = { 0.0f ,0.337f ,0.756f };
	Vector3 noonCenterColor = { 0.501f ,0.623f ,0.776f };
	Vector3 sunsetApexColor = { 0.164f ,0.113f ,0.541f };
	Vector3 sunsetCenterColor = { 0.596f ,0.286f ,0.035f };
	Vector3 duskApexColor = { 0.094f ,0.015f ,0.066f };
	Vector3 duskCenterColor = { 0.125f ,0.109f ,0.239f };
	//Vectors for spherical linear interpolation
	Vector2 Firstquadrant = {1.0f,0.0f};
	Vector2 Secondquadrant = {0.0f,1.0f};
	Vector2 Thirdquadrant = {1.0f,0.0f};
	Vector2 Forthquadrant = {0.0f,1.0f};
	//Difuse colors
	Vector3 dawnDiffuseColor = { 0.866f,0.439f,0.019f };
	Vector3 noonDiffuseColor = { 1.00f,1.00f,1.00f };
	Vector3 sunsetDiffuseColor = { 0.596f ,0.286f ,0.035f };
	Vector3 duskDiffuseColor = { 0.17f ,0.17f ,0.7f };
	Vector3 set,setMoon,setApex,setCenter;
	Vector2 slerpPercent;
	lightDirection = m_Light->GetDirection();

	dZ = 0.0f;
	dX = 0.0f;
	//Radius of the light movement
	radius = sqrtf(pow(1.0f, 2) + pow(1.0f, 2));
	//Angle increments = speed of light, difuse color and gradient color
	angleIncremet = 0.05f;

	if (angle >= 0.0f && angle <= ((90.1f/180.0f) * M_PI) )
	{
		lerpPercent = (angle / ((90.0f / 180.0f) * (float)M_PI));
		//first quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -dZ, 0.5f);

		//Interpolate with spherical linear interpolation and use the Y component as percentage for linear interpolate the colors
		slerpPercent = sLinearInterp->Slerp(Firstquadrant, Secondquadrant, lerpPercent);
		set = linearInterp->Lerp(dawnDiffuseColor, noonDiffuseColor, slerpPercent.y);
		setMoon = linearInterp->Lerp(duskDiffuseColor, dawnDiffuseColor, slerpPercent.y);
		setApex = linearInterp->Lerp(dawnApexColor, noonApexColor, slerpPercent.y);
		setCenter = linearInterp->Lerp(dawnCenterColor, noonCenterColor, slerpPercent.y);

		//m_Light->SetDiffuseColor(set.x,set.y,set.z, 1.0f);
		m_SkyDome->SetApexColor(setApex.x, setApex.y, setApex.z,1.0f);
		m_SkyDome->SetCenterColor(setCenter.x, setCenter.y, setCenter.z, 1.0f);

		angle += (angleIncremet / 180.0f)*  (float)M_PI;
	}
	if (angle > ((90.1f / 180.0f) * M_PI) && angle < ((180.0f / 180.0f) * M_PI))
	{
		if (lerpPercent > 0.99f)
		{
			lerpPercent = 0;
		}
		lerpPercent = (angle/ ((90.f / 180.0f) *(float)M_PI))-1;
		//second quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -dZ, 0.5f);
		//Interpolate with spherical linear interpolation and use the Y component as percentage for linear interpolate the colors
		slerpPercent = sLinearInterp->Slerp(Secondquadrant, Thirdquadrant, lerpPercent);
		set = linearInterp->Lerp(sunsetDiffuseColor, noonDiffuseColor, slerpPercent.y);
		setMoon = linearInterp->Lerp(dawnDiffuseColor, noonDiffuseColor, slerpPercent.y);
		setApex = linearInterp->Lerp(sunsetApexColor, noonApexColor, slerpPercent.y);
		setCenter = linearInterp->Lerp(sunsetCenterColor, noonCenterColor, slerpPercent.y);

		m_Light->SetDiffuseColor(set.x, set.y, set.z, 1.0f);
		m_SkyDome->SetApexColor(setApex.x, setApex.y, setApex.z, 1.0f);
		m_SkyDome->SetCenterColor(setCenter.x, setCenter.y, setCenter.z, 1.0f);

		angle += (angleIncremet / 180.0f)*  (float)M_PI;
	}
	if (angle > ((180.0f / 180.0f) * M_PI) && angle < ((270.0f / 180.0f) * M_PI))
	{
		if (lerpPercent > 0.99f)
		{
			lerpPercent = 0;
		}
		lerpPercent = (angle / ((90.f / 180.0f) * (float)M_PI))-2;
		//third quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -dZ, 0.5f);
		//Interpolate with spherical linear interpolation and use the Y component as percentage for linear interpolate the colors
		slerpPercent = sLinearInterp->Slerp(Thirdquadrant, Forthquadrant, lerpPercent);
		set = linearInterp->Lerp(sunsetDiffuseColor, duskDiffuseColor, slerpPercent.y);
		setMoon = linearInterp->Lerp(sunsetDiffuseColor, duskDiffuseColor, slerpPercent.y);
		setApex = linearInterp->Lerp(sunsetApexColor, duskApexColor, slerpPercent.y);
		setCenter = linearInterp->Lerp(sunsetCenterColor, duskCenterColor, slerpPercent.y);

		//m_Light->SetDiffuseColor(set.x, set.y, set.z, 1.0f);
		m_Light->SetDiffuseColor(setMoon.x, setMoon.y, setMoon.z, 1.0f);
		m_SkyDome->SetApexColor(setApex.x, setApex.y, setApex.z, 1.0f);
		m_SkyDome->SetCenterColor(setCenter.x, setCenter.y, setCenter.z, 1.0f);

		angle += (angleIncremet / 180.0f)*  (float)M_PI;
	}
	if (angle > ((270.0f / 180.0f) * M_PI) && angle <= ((360.0f / 180.0f) * M_PI))
	{
		if (lerpPercent > 0.99f)
		{
			lerpPercent = 0;
		}
		lerpPercent = (angle / ((90.f / 180.0f) * (float)M_PI))-3;
		//fourth quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -dZ, 0.5f);
		//Interpolate with spherical linear interpolation and use the Y component as percentage for linear interpolate the colors
		slerpPercent = sLinearInterp->Slerp(Forthquadrant, Firstquadrant, lerpPercent);
		set = linearInterp->Lerp(dawnDiffuseColor, duskDiffuseColor, slerpPercent.y);
		setMoon = linearInterp->Lerp(duskDiffuseColor, dawnDiffuseColor, slerpPercent.y);
		setApex = linearInterp->Lerp(dawnApexColor, duskApexColor, slerpPercent.y);
		setCenter = linearInterp->Lerp(dawnCenterColor, duskCenterColor, slerpPercent.y);

		//m_Light->SetDiffuseColor(set.x, set.y, set.z, 1.0f);
		m_Light->SetDiffuseColor(setMoon.x, setMoon.y, setMoon.z, 1.0f);
		m_SkyDome->SetApexColor(setApex.x, setApex.y, setApex.z, 1.0f);
		m_SkyDome->SetCenterColor(setCenter.x, setCenter.y, setCenter.z, 1.0f);

		angle += (angleIncremet / 180.0f)*  (float)M_PI;
	}
	if (angle > ((360.0f / 180.0f) * M_PI))
	{
	angle = 0.0f;
	lerpPercent = 0.0f;
	}
	return;
}

bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, OrthoWindowClass* FullScreenWindow)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;

	XMFLOAT3 cameraPosition;

	result = RenderSceneToTexture(Direct3D, ShaderManager, TextureManager);
	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);
	//Get the camera position
	cameraPosition = m_Camera->GetPosition();

	Direct3D->TurnZBufferOff();
	FullScreenWindow->Render(Direct3D->GetDeviceContext());
	m_LightShader->Render(Direct3D->GetDeviceContext(), FullScreenWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, m_DeferredBuffers->GetShaderResourceView(0), m_DeferredBuffers->GetShaderResourceView(1), m_Light->GetDirection());
	
	//Turn the zbuffer and culling back on
	Direct3D->TurnZBufferOn();

	// Render the user interface.
	if(m_displayUI)
	{
		result = m_UserInterface->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
	}
	// Present the rendered scene to the screen.
	Direct3D->EndScene();

	return true;
}

bool ZoneClass::RenderSceneToTexture(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT3 cameraPosition;
	m_DeferredBuffers->SetRenderTargets(Direct3D->GetDeviceContext());
	m_DeferredBuffers->ClearRenderTargets(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	cameraPosition = m_Camera->GetPosition();

	if (m_wireFrame)
	{
		Direct3D->EnableWireframe();
	}
	//Terrain
	m_Terrain->Render(Direct3D->GetDeviceContext());
	ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, TextureManager->GetTexture(0));
	//SkyDome
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	m_SkyDome->Render(Direct3D->GetDeviceContext());
	ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	Direct3D->GetWorldMatrix(worldMatrix);
	if (m_wireFrame)
	{
		Direct3D->DisableWireframe();
	}
	Direct3D->SetBackBufferRenderTarget();
	Direct3D->ResetViewport();

	return true;
}