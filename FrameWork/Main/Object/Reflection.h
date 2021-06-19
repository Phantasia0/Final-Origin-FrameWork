#pragma once

class Reflection
{
private:
	Cam* MirrorCam;
	ID3D11Buffer* VPBuffer;
	RenderTarget* PreTarget;
	CAM_DESC view_desc;
	Texture* ReflectMap;
public:
	Vector2 ClipSize;
	Vector3 SourEye;
	Vector3 DestEye;
	Vector3 Up = Vector3(0, 1, 0);
	Reflection();
	~Reflection();
	void render();
	void PreRender();
};