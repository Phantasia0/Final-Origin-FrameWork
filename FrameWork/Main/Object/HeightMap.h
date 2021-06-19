#pragma once

class HeightMap : public Object
{
private:
	ID3D11Buffer* vertexBuffer;
	VERTEX_PTN* vertex;
	int MaxX, MaxZ;
	float Scale;
	int vertexCount;
	Shader* shader;
	Texture* texture;
public:
	HeightMap(_tstring key = _T("Shader/5.HeightMap.hlsl"), string input = "PTN");
	~HeightMap() override;
	void create(_tstring file, _tstring path = _T(""), float Heightscale = 1.0f, float scale = 1.0f);
	void destroy();
	void update();
	void render();
	MAKEREFGET(Texture*, texture)
	MAKEREFGET(int , MaxX)
	MAKEREFGET(float , Scale)
	MAKEREFGET(VERTEX_PTN*, vertex)
	MAKEREFGET(int , vertexCount)
};