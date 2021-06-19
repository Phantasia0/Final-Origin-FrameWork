#pragma once

class SkySphere : public Object
{
protected:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* IndexBuffer;
	VERTEX_PTN* vertex;
	UINT* index;

	UINT slice;
	UINT stack;

	UINT vertexCount;
	UINT indexCount;

	COL_SPHERE col;
	Shader* shader;
	Texture* texture;
public:
	SkySphere(UINT slice = 20, UINT stack = 20,
		_tstring key = _T("Shader/12.SkySphere.hlsl"),
		string input = "PTN");
	~SkySphere() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;

	MAKEREFGET(COL_SPHERE, col)
	MAKEREFGET(Texture*, texture)
};