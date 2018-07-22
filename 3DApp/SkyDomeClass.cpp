#include "SkyDomeClass.h"

SkyDomeClass::SkyDomeClass()
{
	m_model = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

SkyDomeClass::SkyDomeClass(const SkyDomeClass & other)
{}

SkyDomeClass::~SkyDomeClass()
{}

bool SkyDomeClass::Initialize(ID3D11Device* device)
{
	bool result;
	// load the model
	result = LoadSkyDomeModel("data/3dmodels/skyDome.txt");
	if (!result)
	{
		return false;
	}
	//load into a vertex and index buffer to render
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	//set the color at the top of the skydome rgbAlpha
	//m_apexColor = XMFLOAT4(1.0f, 0.00f, 0.0f, 1.0f);
	//set the color at the center of the skydome (horizon)
	//m_centerColor = XMFLOAT4(0.0f, 0.5f, 0.8f, 1.0f);
	return true;
}

void SkyDomeClass::Shutdown()
{
	//release the vertex buffer and index buffer thet were used for rendering the skydome
	ReleaseBuffers();
	//release skydome model 
	ReleaseSkyDomeModel();
	return;
}

void SkyDomeClass::Render(ID3D11DeviceContext * deviceContext)
{
	RenderBuffers(deviceContext);
	return;
}

int SkyDomeClass::GetIndexCount()
{
	return m_indexCount;
}

XMFLOAT4 SkyDomeClass::GetApexColor()
{
	return m_apexColor;
}

XMFLOAT4 SkyDomeClass::GetCenterColor()
{
	return m_centerColor;
}

void SkyDomeClass::SetApexColor(float red, float green, float blue, float alpha)
{
	m_apexColor= XMFLOAT4(red, green, blue, alpha);
	return;
}

void SkyDomeClass::SetCenterColor(float red, float green, float blue, float alpha)
{
	m_centerColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

bool SkyDomeClass::LoadSkyDomeModel(char * filename)
{
	ifstream fin;
	char input;
	int i;

	//open the file
	fin.open(filename);
	//if it could not open the file then exit
	if (fin.fail())
	{
		return false;
	}
	//read up to the value of vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	//read the vertex count
	fin >> m_vertexCount;
	//set the  number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;
	//Create the model using th vertex xount tht was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}
	//Read up to the beginning of the data
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);
	//Read the vertex data
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}
	//close the model gile
	fin.close();
	return true;
}

void SkyDomeClass::ReleaseSkyDomeModel()
{
	if (!m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
	return;
}

bool SkyDomeClass::InitializeBuffers(ID3D11Device * device)
{
	VertexType * vertices;
	unsigned long * indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	//Create the index Array
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	// load the vertex array and the index array with data
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		indices[i] = i;
	}
	//set up the description of the vertex bufffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	//give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	//Now finally create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//set up the description of the index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	//give the subresource a pinter to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	//create the indexbuffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//Release the arrays now that the vertex buffer and index buffer have been created and loaded
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
	return true;
}

void SkyDomeClass::ReleaseBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void SkyDomeClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set the vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;
	//set the vertexbuffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//set the indebuffer""
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT,0);
	//set the type of primitive that should be rendered from this vertex buffer in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}