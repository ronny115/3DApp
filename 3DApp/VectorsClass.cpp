#include "VectorsClassHeader.h"

Vector3::Vector3()
{}

Vector3::~Vector3()
{}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::Dot(Vector3 start, Vector3 end)
{
	float dot = 0;
	std::vector<float> vectorStart(3);
	std::vector<float> vectorEnd(3);

	vectorStart.at(0) = start.x;
	vectorStart.at(1) = start.y;
	vectorStart.at(2) = start.z;

	vectorEnd.at(0) = end.x;
	vectorEnd.at(1) = end.y;
	vectorEnd.at(2) = end.z;

	dot = (float)std::inner_product(std::begin(vectorStart), std::end(vectorStart), std::begin(vectorEnd), 0.0);
	return dot;
}
float Vector3::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize()
{
	Vector3 vector;
	float length = this->Length();
	if (length != 0)
	{
		vector.x = x / length;
		vector.y = y / length;
		vector.z = z / length;
	}
	return vector;
}

Vector3 Vector3::Lerp(Vector3 start, Vector3 end, float percent)
{
	Vector3 vector;
	vector.x = start.x + percent * (end.x - start.x);
	vector.y = start.y + percent * (end.y - start.y);
	vector.z = start.z + percent * (end.z - start.z);

	return vector;
}

Vector3 Vector3::Nlerp(Vector3 start, Vector3 end, float percent)
{
	return Lerp(start, end, percent).Normalize();
}

Vector3 Vector3::Slerp(Vector3 start, Vector3 end, float percent)
{
	float dotProduct;
	float theta;
	Vector3 vector;
	Vector3 relativeVector;
	dotProduct = vector.Dot(start, end);
	dotProduct = std::max(-1.0f, std::min(dotProduct, 1.0f));
	theta = acosf(dotProduct)*percent;
	relativeVector.x = end.x - (start.x * dotProduct);
	relativeVector.y = end.y - (start.y * dotProduct);
	relativeVector.z = end.z - (start.z * dotProduct);
	relativeVector.Normalize();
	vector.x = ((start.x*cosf(theta)) + (relativeVector.x * sinf(theta)));
	vector.y = ((start.y*cosf(theta)) + (relativeVector.y * sinf(theta)));
	vector.z = ((start.z*cosf(theta)) + (relativeVector.z * sinf(theta)));
	return vector;
}

Vector2::Vector2()
{}

Vector2::~Vector2()
{}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2::Dot(Vector2 start, Vector2 end)
{
	float dot = 0;
	std::vector<float> vectorStart(3);
	std::vector<float> vectorEnd(3);

	vectorStart.at(0) = start.x;
	vectorStart.at(1) = start.y;

	vectorEnd.at(0) = end.x;
	vectorEnd.at(1) = end.y;

	dot = (float)std::inner_product(std::begin(vectorStart), std::end(vectorStart), std::begin(vectorEnd), 0.0);
	return dot;
}
float Vector2::Length()
{
	return sqrtf(x * x + y * y);
}

Vector2 Vector2::Normalize()
{
	Vector2 vector;
	float length = this->Length();
	if (length != 0)
	{
		vector.x = x / length;
		vector.y = y / length;
	}
	return vector;
}

Vector2 Vector2::Lerp(Vector2 start, Vector2 end, float percent)
{
	Vector2 vector;
	vector.x = start.x + percent * (end.x - start.x);
	vector.y = start.y + percent * (end.y - start.y);

	return vector;
}

Vector2 Vector2::Nlerp(Vector2 start, Vector2 end, float percent)
{
	return Lerp(start, end, percent).Normalize();
}

Vector2 Vector2::Slerp(Vector2 start, Vector2 end, float percent)
{
	float dotProduct;
	float theta;
	Vector2 vector;
	Vector2 relativeVector;
	dotProduct = vector.Dot(start, end);
	dotProduct = std::max(-1.0f, std::min(dotProduct, 1.0f));
	theta = acosf(dotProduct)*percent;
	relativeVector.x = end.x - (start.x * dotProduct);
	relativeVector.y = end.y - (start.y * dotProduct);
	relativeVector.Normalize();
	vector.x = ((start.x*cosf(theta)) + (relativeVector.x * sinf(theta)));
	vector.y = ((start.y*cosf(theta)) + (relativeVector.y * sinf(theta)));
	return vector;
}