#pragma once

class Terrain : public Object
{
private:
	int MaxX = 257;
	float ScaleX = 1.0f;

	VERTEX_PTN* vertex;
	ID3D11Buffer* vertexBuffer;
	UINT* index;
	int vertexCount;
	int indexCount;

	Shader* shader;
	Texture* texture;

	struct InputDesc
	{
		UINT index;
		Vector3 v0, v1, v2;
	};

	struct OutputDesc
	{
		int picked;
		float u, v, distance;
	};

	InputDesc* InputArray;
	ID3D11Resource* input;
	ID3D11ShaderResourceView* srv;

	OutputDesc* OutputArray;
	ID3D11Resource* output;
	ID3D11UnorderedAccessView* uav;

	ID3D11Resource* result;

	struct RayBuffer
	{
		Vector3 position;
		float size;
		Vector3 direction;
		float padding;
	}ray;

	ID3D11Buffer* rayBuffer;

	Shader* compute;
	float* Height;
	UINT size;

	void CreateStrutedBuffer();
	void DeleteStrutedBuffer();
	void DeleteVertex();
	void CreateVertex();
public:
	friend class QuadTree;
	friend class TerrainEditor;
	Terrain(_tstring key = _T("Shader/5.HeightMap.hlsl"), string input = "PTN");
	~Terrain() override;
	HRESULT create() override;
	void destroy() override;
	void update() override;
	void render() override;
	bool ComPutePicking(Ray wRay, OUT Vector3& pos);
	void SaveTerrain(string file);
	void LoadTerrain(string file);

	MAKEREFGET(Texture*, texture)
	MAKEREFGET(int, MaxX)
	MAKEREFGET(float, ScaleX)
	MAKEREFGET(VERTEX_PTN*, vertex)
	MAKEREFGET(int, vertexCount)
	MAKEREFGET(Shader*, shader)
};