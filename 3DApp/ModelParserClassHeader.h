#include <iostream>
#include <fstream>
using namespace std;

class ModelParserClass
{
private:
	struct VertexType
	{
		float x, y, z;
	};
	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};
	VertexType * vertices, *texcoords, *normals;
	FaceType *faces;
public:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	ModelType * model;
	ModelParserClass();
	ModelParserClass(const ModelParserClass&);
	~ModelParserClass();

	bool LoadDataStructures(char*, int&, int&, int&, int&);

};