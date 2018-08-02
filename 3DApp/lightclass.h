#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <directxmath.h>
using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT3 GetPosition();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();
private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_diffuseColor2;
	XMFLOAT3 m_direction;
	XMFLOAT3 m_direction2;
	XMFLOAT3 m_position;
	XMFLOAT4 m_specularColor;
	float m_specularPower;
};

#endif