#include "SkyDomeShaderClass.h"

SkyDomeShaderClass::SkyDomeShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_colorBuffer = 0;
}

SkyDomeShaderClass::SkyDomeShaderClass(const SkyDomeShaderClass & other)
{}

SkyDomeShaderClass::~SkyDomeShaderClass()
{}

bool SkyDomeShaderClass::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result;
	//initialize the pixel and vertex shader
	result = InitializeShader(device, hwnd, L"SkyDome.vs", L"SkyDome.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void SkyDomeShaderClass::Shutdown()
{
	//shutdown the vertex and pixel shaders a swell as the related objects
	ShutdownShader();
	return;
}

bool SkyDomeShaderClass::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	bool result;
	//set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
	if (!result)
	{
		return false;
	}
	//now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);
	return true;
}

bool SkyDomeShaderClass::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob * errorMessage;
	ID3D10Blob * vertexShaderBuffer;
	ID3D10Blob * pixelSahderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC colorBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelSahderBuffer = 0;

	//Compile the vertex shader
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "SkyDomeVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}
	//Compile the pixel shader
	result = D3DCompileFromFile(psFilename, NULL, nullptr, "SkyDomePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelSahderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}
	//Create the vertex shader buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	//Create the pixel shader buffer
	result = device->CreatePixelShader(pixelSahderBuffer->GetBufferPointer(), pixelSahderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	//Create the vertex input Layout description
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	//get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	//Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}
	//Release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelSahderBuffer->Release();
	pixelSahderBuffer = 0;
	//Setup the description of the dynaic constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	//Create the constant buffer pointer so we can access the vertex sahder constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//Steup the desc of the dynamic pixel constant buffer that is in the pixel shader
	colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesc.ByteWidth = sizeof(ColorBufferType);
	colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;
	//Create the pixel constant buffer pointer so we can acccess the pixel shader constant buffer from within this class
	result = device->CreateBuffer(&colorBufferDesc, NULL, &m_colorBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void SkyDomeShaderClass::ShutdownShader()
{
	// Release the pixel constant buffer.

	if (m_colorBuffer)
	{
		m_colorBuffer->Release();
		m_colorBuffer = 0;
	}
	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	return;
}

void SkyDomeShaderClass::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
	char * compileErrors;
	unsigned __int64 bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();
	fout.open("shader-error.txt");
	//write the error message
	for(i=0;i<bufferSize;i++)
	{
		fout << compileErrors[i];
	}
	fout.close();
	//release the error message
	errorMessage->Release();
	errorMessage = 0;
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message", shaderFilename, MB_OK);
	return;
}

bool SkyDomeShaderClass::SetShaderParameters(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType * dataPtr;
	unsigned int bufferNumber;
	ColorBufferType * dataPtr2;

	//Transponse the matrices to preapre them for the shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	//lock the matrix constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	//get a pointer to the data in the matrix constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	//copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	//unlock the matrix constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);
	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;
	//Now set the matrix constant buffer in the matrix vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//lock the color buffer 
	result = deviceContext->Map(m_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	dataPtr2 = (ColorBufferType*)mappedResource.pData;
	dataPtr2->apexColor = apexColor;
	dataPtr2->centerColor = centerColor;

	deviceContext->Unmap(m_colorBuffer, 0);
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_colorBuffer);
	return true;
}

void SkyDomeShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	//set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);
	//set the vertex and pixel shader
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	//render the triangles
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}