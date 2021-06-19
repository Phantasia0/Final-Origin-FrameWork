#pragma once

struct cUV
{
	Vector2 MinUV;
	Vector2 MaxUV;
};

class Ui : public Object
{
private:
	ID3D11Buffer* vertexBuffer;
	VERTEX_PTN* vertex;
	Shader* shader;
	Texture* texture;
	bool isActive;
	ID3D11Buffer* UVBuffer;
public:
	cUV UV;

	UINT MaxFrameX = 1;
	UINT MaxFrameY = 1;
	int CurrentFrameX = 0;
	int CurrentFrameY = 0;

	vector<pair<string, Ui*>> vecChild;

	function<void()> click;
	function<void()> mouseOver;
	function<void()> drag;

	bool Click(WPARAM keyCode);
	bool Drag(WPARAM keyCode);
	bool MouseOver();

	Ui(_tstring key = _T("Shader/9.Ui.hlsl"), string input = "PTN");
	~Ui() override;
	HRESULT create(float minU, float minV, float maxU, float maxV);
	void destroy() override;
	void update() override;
	void render() override;

	MAKEGET(bool, isActive)
	MAKEREFGET(Texture*, texture)
	MAKEREFGET(VERTEX_PTN*, vertex)
};