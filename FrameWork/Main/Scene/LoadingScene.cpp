#include "stdafx.h"
#include "LoadingScene.h"
#include "Scene1.h"

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

HRESULT LoadingScene::init()
{
	t1 = new thread(LoadScene);

	Loading = new Ui();
	Loading->create(0, 0, 1, 1);
	Loading->GetReftexture()->CreateTexture(_T("load.png"), _T("../../Asset/"));
	Loading->GetRefScale() = Vector3(300, 300, 1);
	Loading->GetRefPos() = Vector3(0, 0, 1.0f);

	return S_OK;
}

void LoadingScene::release()
{
	Loading->destroy();
	SAFE_DELETE(Loading);
}

void LoadingScene::update()
{
	Loading->update();
}

void LoadingScene::PreRender()
{
}

void LoadingScene::render()
{
	Loading->render();
}

void LoadingScene::LoadScene()
{
	Scene1* temp = new Scene1();
	temp->init();

	SCENEMANAGER->AddScene("SCENE1", temp);
	SCENEMANAGER->ChangeScene("SCENE1");
}