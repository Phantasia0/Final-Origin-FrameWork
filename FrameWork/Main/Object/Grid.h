#pragma once

class Grid : public Object
{
private:
	ID3D11Buffer* vertexBuffer;
	VERTEX_PC* vertex;
	int MaxX, MaxZ;
	float Scale;
	int vertexCount;
	Shader* shader;
public:
	Grid(int x = 20, int z = 20, float scale = 1.0f,
		_tstring key = _T("Shader/1.Basic.hlsl"),
		string input = "PC");
	~Grid() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
};