#pragma once

struct DirLight
{
	Vector3 Dir;
	float Padding;
};

struct PointLight
{
	Vector3 Pos;
	float Power;
};

class LightManager : public SingletonBase<LightManager>
{
private:
	DirLight dirLight;
	ID3D11Buffer* DirLightBuffer;
public:
	LightManager();
	~LightManager();
	void render();
	MAKEREFGET(DirLight, dirLight)
};