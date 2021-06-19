#pragma once

class Perspective : public Projection
{
private:
	float aspect;
public:
	Perspective(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = (float)D3DX_PI * 0.25f);
	~Perspective();
	void Set(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = (float)D3DX_PI * 0.25f);
	void Set(Matrix* mat);
};