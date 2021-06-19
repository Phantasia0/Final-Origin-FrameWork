#pragma once

class Cube : public Object
{
protected:
	ID3D11Buffer* vertexBuffer;
	VERTEX_PCN* vertex;
	COL_CUBE col;
public:
	Shader* shader;
	Cube(_tstring key = _T("Shader/3.Light.hlsl"), string input = "PCN");
	~Cube() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
	MAKEREFGET(COL_CUBE, col)
	MAKEREFGET(VERTEX_PCN*, vertex)
};