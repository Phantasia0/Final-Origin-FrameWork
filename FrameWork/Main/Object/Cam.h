#pragma once

class Cam : public Object
{
private:
	View* V;
	Perspective* Proj;
	Orthographic* O;
	Viewport* VP;
	Vector3 WorldEye;
	Vector3 _Eye, _Look, _Up;
	ID3D11Buffer* ViewBuffer;
	ID3D11Buffer* ProjectionBuffer;
	ID3D11Buffer* OrthoProjectionBuffer;
	ID3D11Buffer* ViewPosBuffer;
	float zn = 0.1f;
	float zf = 100000.0f;
	float fov = (float)D3DX_PI * 0.25f;
public:
	Cam();
	~Cam() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;

	MAKEREFGET(View*, V)
	MAKEREFGET(Perspective*, Proj)
	MAKEREFGET(Orthographic*, O)
	MAKEREFGET(Viewport*, VP)
	MAKEREFGET(Vector3, _Eye)
	MAKEREFGET(Vector3, WorldEye)
	MAKEREFGET(float, fov)
	MAKEREFGET(float, zn)
	MAKEREFGET(float, zf)
};