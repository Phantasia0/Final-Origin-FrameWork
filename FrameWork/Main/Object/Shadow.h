#pragma once

struct CAM_DESC
{
	Matrix V, P;
};

class Shadow
{
public:
	CAM_DESC view_desc;

	struct SHADOW_DESC
	{
		Vector2 size;
		float bias;
		int selected;
		SHADOW_DESC()
		{
			size = Vector2(0, 0);
			bias = 0.0005f;
			selected = 0;
		}
	} shadow_desc;
	Shadow();
	~Shadow();
	void render();
	ID3D11Buffer* VPBuffer;
	ID3D11Buffer* ShadowBuffer;
	Texture* DepthMap;
};