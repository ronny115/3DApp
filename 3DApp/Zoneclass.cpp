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
}

ZoneClass::ZoneClass(const ZoneClass& other)
{
}

ZoneClass::~ZoneClass()
{
}

bool ZoneClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	bool result;

	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if(!m_UserInterface)
	{
		return false;
	}

	// Initialize the user interface object.
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

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, -0.5f);

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation the terrain x(from 0 to 256)and z(0 to 256).
	m_Position->SetPosition(128.0f, 10.0f, 128.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(Direct3D->GetDevice(),"data/setup.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Set the UI to display by default.
	m_displayUI = true;

	//set wire frame rendering to enabled
	m_wireFrame = false;

	return true;
}

void ZoneClass::Shutdown()
{
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

bool ZoneClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, float frameTime, int fps, int cpu)
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
	result = Render(Direct3D, ShaderManager, TextureManager);
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
	float dX, dZ, radius;
	XMFLOAT3 lightDirection;

	lightDirection = m_Light->GetDirection();
	dZ = 0.0f;
	dX = 0.0f;

	radius = sqrtf(pow(lightDirection.x,2) + pow(lightDirection.z, 2));
	//Angle increments = speed of rotation
	angle += (0.1f / 180.0f)*  M_PI;

	if (angle >= 0.0f && angle < ((90.0f/180.0f) * M_PI) )
	{
		//first quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -1.0f, dZ);
	}
	if (angle > ((90.0f / 180.0f) * M_PI) && angle < ((180.0f / 180.0f) * M_PI))
	{
		//second quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -1.0f, dZ);
	}
	if (angle > ((180.0f / 180.0f) * M_PI) && angle < ((270.0f / 180.0f) * M_PI))
	{
		//third quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -1.0f, dZ);
	}
	if (angle > ((270.0f / 180.0f) * M_PI) && angle <= ((360.0f / 180.0f) * M_PI))
	{
		//fourth quadrant
		dX = 0.0f + cosf(angle)*radius;
		dZ = 0.0f + sinf(angle)*radius;
		m_Light->SetDirection(dX, -1.0f, dZ);		
	}
	if (angle > ((360.0f / 180.0f) * M_PI))
	{
	angle = 0.0f;
	}
	return;
}

bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	
	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//turn on the wire frame rendering of the terrain if needed
	if (m_wireFrame)
	{
		Direct3D->EnableWireframe();
	}

	// Render the terrain
	//result = ShaderManager->RenderColorShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, 
	//										  projectionMatrix);
	//result = ShaderManager->RenderTextureShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
	//										  projectionMatrix, TextureManager->GetTexture(1));

	m_Terrain->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, TextureManager->GetTexture(0), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	if(!result)
	{
		return false;
	}

	//turn off wire frame rendering of the terrain if it was on
	if (m_wireFrame)
	{
		Direct3D->DisableWireframe();
	}

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