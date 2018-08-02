#include "shadermanagerclass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_FontShader = 0;
	m_TerrainShader = 0;
	m_SkyDomeShader = 0;
}


ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other)
{
}


ShaderManagerClass::~ShaderManagerClass()
{
}


bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//create and initialize the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}
	result = m_ColorShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	//create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	//create and initialize the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}
	result = m_LightShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	//create and initialize the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}
	//create and initialize the terrain shader object
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}
	result = m_TerrainShader->Initialize(device, hwnd);
	if(!result)
	{
		return false;
	}

	//create and initialize the terrain shader object
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}
	result = m_SkyDomeShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}


void ShaderManagerClass::Shutdown()
{
	//release the skydome shader object
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}
	//release the terrain shader object
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}
	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	 //Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	return;
}


bool ShaderManagerClass::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	return m_ColorShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderManagerClass::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_TextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* colorRexture, ID3D11ShaderResourceView* normalRexture, XMFLOAT3 lightDirection)
{
	return m_LightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, colorRexture, normalRexture, lightDirection);
}

bool ShaderManagerClass::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	return m_FontShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}

bool ShaderManagerClass::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	return m_TerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManagerClass::RenderSkyDomeShader(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	return m_SkyDomeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
}