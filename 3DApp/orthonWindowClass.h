#ifndef _ORTHOWINDOWCLASS_H_
#define _ORTHOWINDOWCLASS_H_
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class OrthoWindowClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
public:
	OrthoWindowClass();
	OrthoWindowClass(const OrthoWindowClass&);
	~OrthoWindowClass();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
};
#endif // !_ORTHOWINDOWCLASS_H_

