#pragma once

class Sphere : public Object
{
protected:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* IndexBuffer;
	VERTEX_PCN* vertex;
	UINT* index;

	UINT slice;
	UINT stack;
	UINT vertexCount;
	UINT indexCount;
	COL_SPHERE col;
	Shader* shader;
public:
	Sphere(UINT slice = 20, UINT stack = 20,
		_tstring key = _T("Shader/3.Light.hlsl"),
		string input = "PCN");
	~Sphere() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
	MAKEREFGET(COL_SPHERE, col)
};