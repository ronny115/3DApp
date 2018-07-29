#include "ModelParserClassHeader.h"

ModelParserClass::ModelParserClass()
{}
ModelParserClass::ModelParserClass(const ModelParserClass& other)
{}
ModelParserClass::~ModelParserClass()
{}

bool ModelParserClass::LoadDataStructures(char* filename, int & vertexCount, int & textureCount, int & normalCount, int & faceCount)
{
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex,ind;
	char input, input2;
	//Read the counts

	fin.open(filename);
	if (fin.fail() == true)
	{
		return false;
	}

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}
		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}
	fin.close();
	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}
	model = new ModelType[(faceCount * 3)+1];
	if (!model)
	{
		return false;
	}
	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;
	ind = 0;
	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;
				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i<faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		model[ind].x = vertices[vIndex].x;
		model[ind].y = vertices[vIndex].y;
		model[ind].z = vertices[vIndex].z;

		model[ind].tu = vertices[tIndex].x;
		model[ind].tv = vertices[tIndex].y;

		model[ind].nx = vertices[nIndex].x;
		model[ind].ny = vertices[nIndex].y;
		model[ind].nz = vertices[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		model[ind + 1].x = vertices[vIndex].x;
		model[ind + 1].y = vertices[vIndex].y;
		model[ind + 1].z = vertices[vIndex].z;

		model[ind + 1].tu = vertices[tIndex].x;
		model[ind + 1].tv = vertices[tIndex].y;

		model[ind + 1].nx = vertices[nIndex].x;
		model[ind + 1].ny = vertices[nIndex].y;
		model[ind + 1].nz = vertices[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		model[ind + 2].x = vertices[vIndex].x;
		model[ind + 2].y = vertices[vIndex].y;
		model[ind + 2].z = vertices[vIndex].z;

		model[ind + 2].tu = vertices[tIndex].x;
		model[ind + 2].tv = vertices[tIndex].y;

		model[ind + 2].nx = vertices[nIndex].x;
		model[ind + 2].ny = vertices[nIndex].y;
		model[ind + 2].nz = vertices[nIndex].z;

		ind += 3;
	}

	//Release the data structures
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	return true;
}


