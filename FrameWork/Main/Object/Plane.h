#pragma once

class Plane : public Object
{
private:
	ID3D11Buffer* vertexBuffer;
	VERTEX_PTN* vertex;
	Shader* shader;
	Texture* texture;
public:
	Plane(_tstring key = _T("Shader/4.Plane.hlsl"), string input = "PTN");
	~Plane() override;
	HRESULT create(float minU, float minV, float maxU, float maxV);
	void destroy() override;
	void update() override;
	void render() override;
	MAKEREFGET(Texture*, texture)
	MAKEREFGET(VERTEX_PTN*, vertex)
};