#pragma once

class Viewport
{
private:
	float x, y;
	float width, height;
	float minDepth, maxDepth;
	D3D11_VIEWPORT viewport;
public:
	Viewport(float width, float height, float x=0, float y=0, float minDepth=0,float maxDepth=1);
	~Viewport();
	void RSSetViewport();
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetX() { return x; }
	float GetY() { return y; }
};