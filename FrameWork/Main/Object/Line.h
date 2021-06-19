#pragma once

class Line : public Object
{
protected:
	ID3D11Buffer* vertexBuffer;
	Shader* shader;
public:
	VERTEX_PC vertex[2];

	Line(_tstring key = _T("Shader/2.Basic2.hlsl"), string input = "PC");
	~Line() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
};