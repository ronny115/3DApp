#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
using namespace DirectX;
using namespace std;

class TerrainClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
		XMFLOAT3 color;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
		float r, g, b;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool LoadSetupFile(char*);
	bool LoadBitmapHeightMap();
	bool LoadRawHeightMap();
	bool LoadColorMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool CalculateNormals();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

	void CalculatTerrainVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_terrainHeight, m_terrainWidth;
	float m_heightScale;
	char *m_terrainFilename, *m_colorMapFilename;
	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;
};

#endif