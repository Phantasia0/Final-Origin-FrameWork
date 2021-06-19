#pragma once

struct BlurDesc
{
	int count;
	float width;
	float height;
	int select;

	BlurDesc()
	{
		count = 1;
		width = 0.0f;
		height = 0.0f;
		select = 0;
	}
};

class Blur : public PostEffect
{
private:
	ID3D11Buffer* blurBuffer;
	BlurDesc blurDesc;
public:
	Blur(_tstring key = _T("Shader/14.Blur.hlsl"), string input = "PT");
	~Blur() override;
	void render() override;
	MAKEREFGET(BlurDesc,blurDesc)
};