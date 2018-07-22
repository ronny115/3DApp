#pragma once
#include <math.h>
#include "stdafx.h"
#include <vector>
#include <numeric>
#include <algorithm>

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();

	float Length();
	float Dot(Vector3, Vector3);
	Vector3 Normalize();
	Vector3 Lerp(Vector3, Vector3, float);
	Vector3 Slerp(Vector3, Vector3, float);
	Vector3 Nlerp(Vector3, Vector3, float);
	float x, y, z;
};

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();

	float Length();
	float Dot(Vector2, Vector2);
	Vector2 Normalize();
	Vector2 Lerp(Vector2, Vector2, float);
	Vector2 Slerp(Vector2, Vector2, float);
	Vector2 Nlerp(Vector2, Vector2, float);
	float x, y;
};