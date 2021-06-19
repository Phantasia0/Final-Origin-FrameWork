#pragma once

struct Mtl
{
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	Vector4 Emissive;
	Vector4 HasMap;

	Mtl() : HasMap(0.0f, 0.0f, 0.0f, 0.0f), Ambient(0.3f, 0.3f, 0.3f, 1),
		Diffuse(1, 1, 1, 1), Specular(1, 1, 1, 1), Emissive(0.0f, 0.0f, 0.0f, 1.0f)
	{};
};

class Material
{
private:
	ID3D11Buffer* MaterialBuffer;
public:
	Material();
	~Material();
	void update();
	void render();
	Mtl mtl;
};

