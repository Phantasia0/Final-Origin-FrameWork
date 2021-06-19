#pragma once

class PostEffect : public Object
{
protected:
	VERTEX_PT* vertices;
	ID3D11Buffer* vertexBuffer;
	Shader* shader;
	Texture* texture;
public:
	PostEffect(_tstring key = _T("Shader/13.PostEffect.hlsl"),
		string input = "PT");
	~PostEffect() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
	MAKEREFGET(Texture*, texture)
};