#pragma once

struct RAIN_DESC
{
	Vector3 velocity;
	float padding1;
	Color color;
	Vector3 origin;
	float time;
	Vector3 size;
	float padding2;

	RAIN_DESC()
	{
		velocity = Vector3(0, -1, 0);
		color = Color(1, 1, 1, 1);
		origin = Vector3(0, 0, 0);
		size = Vector3(0, 0, 0);
		time = 0.0f;
	}
};

class Rain : public Particle
{
private:
	RAIN_DESC RainDesc;
	ID3D11Buffer* RainBuffer;
	ID3D11Buffer* vertexBuffer = nullptr;
	vector<VERTEX_PS> vertices;

	ID3D11Buffer* ViewBuffer;
	ID3D11Buffer* ProjectionBuffer;
	ID3D11Buffer* ViewPosBuffer;

	Vector3 size = Vector3(1000, 1000, 1000);
	Vector4 minmax = Vector4(4.0f, 8.0f, 4.0f, 8.0f);
public:
	Rain(UINT particleCount = 1, _tstring key = _T("Shader/15.Rain.hlsl"), string input = "PS");
	~Rain() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
	void reset() override;

	MAKEREFGET(Vector3, size)
	MAKEREFGET(Vector4, minmax)
};