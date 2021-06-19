#pragma once

class TerrainEditor
{
public:
	struct TerrainBrush
	{
		float Range = 1.0f;
		float Weight = 1.0f;
		int Type = 0;
		float Maxheight = 10.0f;
	}brush;

	Terrain* terrain;

	TerrainEditor();
	~TerrainEditor();
	void EditHeight(Vector3 Pos);
	void ResetBuffer();
	void ResetVertexBuffer();
};