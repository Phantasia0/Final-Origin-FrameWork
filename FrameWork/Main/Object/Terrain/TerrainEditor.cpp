#include "stdafx.h"
#include "TerrainEditor.h"

TerrainEditor::TerrainEditor()
{
}

TerrainEditor::~TerrainEditor()
{
}

void TerrainEditor::EditHeight(Vector3 Pos)
{
	if (brush.Type < 3)
	{
		for (int i = 0; i < terrain->vertexCount; i++)
		{
			Vector3 v1 = Vector3(Pos.x, 0.0f, Pos.z);
			Vector3 v2 = Vector3(terrain->vertex[i].Position.x, 0.0f, terrain->vertex[i].Position.z);
			v2 = v2 - v1;

			float Dis = D3DXVec3Length(&v2);
			float w = Dis / brush.Range;
			Saturate(w);
			w = 1.0f - w;

			if (brush.Type == 1)
			{
				w *= PI * 0.5f;
				w = sinf(w);
			}

			if (brush.Type == 2)
			{
				if (w > 0.0f) w = 1.0f;
			}

			terrain->vertex[i].Position.y += w * brush.Weight * DeltaTime;
			if (terrain->vertex[i].Position.y > brush.Maxheight)
			{
				terrain->vertex[i].Position.y = brush.Maxheight;
			}
			else if (terrain->vertex[i].Position.y < 0.0f)
			{
				terrain->vertex[i].Position.y = 0.0f;
			}
		}
	}

	g_DeviceContext->UpdateSubresource(terrain->vertexBuffer, 0, NULL, terrain->vertex, 0, 0);
}

void TerrainEditor::ResetBuffer()
{
	terrain->DeleteStrutedBuffer();
	terrain->CreateStrutedBuffer();
}

void TerrainEditor::ResetVertexBuffer()
{
	terrain->DeleteVertex();
	terrain->CreateVertex();
}