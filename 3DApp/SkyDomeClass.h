#ifndef _SKYDOMECLASS_H_
#define _SKYDOMECLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "ModelParserClassHeader.h"

using namespace DirectX;
using namespace std;

class SkyDomeClass
{
private:
	
	struct VertexType
	{
		XMFLOAT3 position;
	};
	bool LoadSkyDomeModel(char*);
	void ReleaseSkyDomeModel();
	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	ModelParserClass::ModelType * m_model;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	XMFLOAT4 m_apexColor, m_centerColor;

public:
	SkyDomeClass();
	SkyDomeClass(const SkyDomeClass&);
	~SkyDomeClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	XMFLOAT4 GetApexColor();
	XMFLOAT4 GetCenterColor();
	void SetApexColor(float, float, float, float);
	void SetCenterColor(float, float, float, float);
};
#endif