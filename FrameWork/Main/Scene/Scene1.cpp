#include "stdafx.h"
#include "Scene1.h"

Scene1::Scene1()
{
	SOUNDMANAGER->stop("BGM");
}

Scene1::~Scene1()
{
}

HRESULT Scene1::init()
{
	Quad = new QuadTree();
	Quad->terrain = new Terrain();
	Quad->terrain->create();
	Quad->create();

	Quad->terrain->GetReftexture()->CreateTexture(_T("terrain.jpg"), _T("../../Asset/HeightMap/"));
	Editor = new TerrainEditor();
	Editor->terrain = Quad->terrain;

	return S_OK;
}

void Scene1::release()
{
	Quad->destroy();
	SAFE_DELETE(Quad);
}

void Scene1::update()
{
	ImGui::SliderInt("BrushType", &Editor->brush.Type, 0, 2);
	ImGui::SliderFloat("BrushWeight", &Editor->brush.Weight, -100.0f, 100.0f);
	ImGui::SliderFloat("BrushRange", &Editor->brush.Range, 0.0f, 200.0f);
	ImGui::SliderFloat("BrushHeight", &Editor->brush.Maxheight, 0.0f, 200.0f);

	if (ImGui::SliderFloat("TerrainSize", &Quad->terrain->GetRefScaleX(), 0.0f, 200.0f))
	{
		Editor->ResetVertexBuffer();
		Quad->destroy();
		Quad->create();
		Editor->ResetBuffer();
	}

	if (ImGui::Button("Max129"))
	{
		Quad->destroy();
		Quad->terrain->GetRefMaxX() = 129;
		Editor->ResetVertexBuffer();
		Quad->create();
		Editor->ResetBuffer();
	}

	if (ImGui::Button("Max257"))
	{
		Quad->destroy();
		Quad->terrain->GetRefMaxX() = 257;
		Editor->ResetVertexBuffer();
		Quad->create();
		Editor->ResetBuffer();
	}

	if (ImGui::Button("Save"))
	{
		Quad->terrain->SaveTerrain("../../Asset/Save/Terrain1.txt");
	}

	if (ImGui::Button("Load"))
	{
		Quad->destroy();
		Quad->terrain->LoadTerrain("../../Asset/Save/Terrain1.txt");
		Editor->ResetVertexBuffer();
		Quad->create();
		Editor->ResetBuffer();
	}

	if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
	{
		Ray wMouse = ClacPickingRay(g_ptMouse, CAMMANAGER->vec_pCam[0]);
		Vector3 Target;

		if (Quad->terrain->ComPutePicking(wMouse, Target))
		{
			Editor->EditHeight(Target);
		}
	}

	if (KEYMANAGER->isKeyUp(VK_LBUTTON))
	{
		Editor->ResetBuffer();
	}

	Quad->update();
}

void Scene1::PreRender()
{
	CAMMANAGER->vec_pCam[0]->render();
}

void Scene1::render()
{
	Quad->render();
}