#pragma once

class Refraction
{
private:
	Cam* MirrorCam;
	ID3D11Buffer* VPBuffer;
	RenderTarget* PreTarget;
	CAM_DESC view_desc;
	Texture* RefractMap;

	ID3D11Buffer* TimeBuffer;
	Vector4 time;
	Texture* NormalMap;
public:
	Vector2 ClipSize;
	Vector3 SourEye;
	Vector3 DestEye;
	Vector3 Up = Vector3(0, 1, 0);
	Refraction(_tstring normalfile, _tstring path = _T(""));
	~Refraction();
	void render();
	void PreRender();
};