#include "stdafx.h"
#include "MainGame.h"
#include "Scene\Scene1.h"
#include "Scene\LoadingScene.h"

MainGame::MainGame()
{

}

MainGame::~MainGame()
{
}

HRESULT MainGame::init()
{
	srand(static_cast<UINT>(time(NULL)));
	LIGHTMANAGER;

	SOUNDMANAGER->addSound("BGM", "bgm.wav", true);
	SOUNDMANAGER->play("BGM", 1.0f);

	Scene* temp = new LoadingScene();
	temp->init();
	SCENEMANAGER->AddScene("loading", temp);
	SCENEMANAGER->ChangeScene("loading");

	ImGui::Create(g_D3dDesc.Handle, g_Device, g_DeviceContext);
	ImGui::StyleColorsDark();
	g_D3dDesc.bVsync = false;

	return S_OK;
}

void MainGame::release()
{
	KEYMANAGER->ReleaseSingleton();
	TIMEMANAGER->ReleaseSingleton();
	CAMMANAGER->ReleaseSingleton();
	SHADERMANAGER->DeleteAll();
	SHADERMANAGER->ReleaseSingleton();
	SCENEMANAGER->ReleaseSingleton();
	SOUNDMANAGER->ReleaseSingleton();

	ImGui::Delete();
}

void MainGame::update()
{
	ImGui::Update();
	TIMEMANAGER->Render();
	KEYMANAGER->Update();
	SCENEMANAGER->CurrentScene.second->update();
	CAMMANAGER->update();
	SOUNDMANAGER->Update();
}

void MainGame::PreRender()
{
	LIGHTMANAGER->render();
	CAMMANAGER->vec_pCam[0]->render();
	SCENEMANAGER->CurrentScene.second->PreRender();
}

void MainGame::render()
{
	SCENEMANAGER->CurrentScene.second->render();
	ImGui::Render();
}