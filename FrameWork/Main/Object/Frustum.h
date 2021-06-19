#pragma once

class Frustum
{
private:
	D3DXPLANE planes[6];
	Cam* pCam;
	float a, b, c, d;
public:
	Frustum();
	~Frustum();
	void Update();
	bool ContainPoint(Vector3 point);
	bool ContainSphere(Vector3 center, float radius);
	MAKEREFGET(Cam*, pCam)
};