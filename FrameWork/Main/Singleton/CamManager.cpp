#include "stdafx.h"
#include "CamManager.h"

CamManager::CamManager()
{
	Cam* MainCam = new Cam();
	MainCam->create();
	AddCamList(MainCam);
}

CamManager::~CamManager()
{
	for (UINT i = 0; i < vec_pCam.size(); i++)
	{
		vec_pCam[i]->destroy();
		SAFE_DELETE(vec_pCam[i]);
	}
	vec_pCam.clear();
	vec_pCam.shrink_to_fit();
}

void CamManager::AddCamList(Cam * p)
{
	vec_pCam.emplace_back(p);
}

void CamManager::update()
{
	if (KEYMANAGER->StayKeyDown('W'))
	{
		CAMMANAGER->vec_pCam[0]->GetRefPos() 
			+= 3.0f * CAMMANAGER->vec_pCam[0]->GetRefForward() *DeltaTime;
	}
	if (KEYMANAGER->StayKeyDown('S'))
	{
		CAMMANAGER->vec_pCam[0]->GetRefPos()
			-= 3.0f * CAMMANAGER->vec_pCam[0]->GetRefForward() * DeltaTime;
	}
	if (KEYMANAGER->StayKeyDown('A'))
	{
		CAMMANAGER->vec_pCam[0]->GetRefPos()
			-= 3.0f * CAMMANAGER->vec_pCam[0]->GetRefRight() * DeltaTime;
	}

	if (KEYMANAGER->StayKeyDown('D'))
	{
		CAMMANAGER->vec_pCam[0]->GetRefPos()
			+= 3.0f * CAMMANAGER->vec_pCam[0]->GetRefRight() * DeltaTime;
	}
	if (KEYMANAGER->StayKeyDown('Q'))
	{
		CAMMANAGER->vec_pCam[0]->GetRefPos()
			+= 3.0f * CAMMANAGER->vec_pCam[0]->GetRefUp() * DeltaTime;
	}
	if (KEYMANAGER->StayKeyDown('E'))
	{
		CAMMANAGER->vec_pCam[0]->GetRefPos()
			-= 3.0f * CAMMANAGER->vec_pCam[0]->GetRefUp() * DeltaTime;
	}

	for (UINT i = 0; i < vec_pCam.size(); i++)
	{
		vec_pCam[i]->update();
	}
}

void CamManager::render()
{
	for (UINT i = 0; i < vec_pCam.size(); i++)
	{
		vec_pCam[i]->render();
	}
}

void CamManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
		{
			Vector3 EyeLook;
			EyeLook = -CAMMANAGER->vec_pCam[0]->GetRef_Eye();
			D3DXVec3Normalize(&EyeLook, &EyeLook);
			CAMMANAGER->vec_pCam[0]->GetRef_Eye() +=
				EyeLook * GET_WHEEL_DELTA_WPARAM(wParam) / 30.0f;
		}
			break;
		case WM_RBUTTONDOWN:
			ptPrevMouse.x = LOWORD(lParam);
			ptPrevMouse.y = HIWORD(lParam);
			isButtonDown = true;
			break;
		case WM_RBUTTONUP:
			isButtonDown = false;
			CamRotAngle.y = 0.0f;
			CamRotAngle.x = 0.0f;
			break;
		case WM_MOUSEMOVE:
			g_ptMouse.x = LOWORD(lParam);
			g_ptMouse.y = HIWORD(lParam);

			if (isButtonDown)
			{
				POINT ptCurrMouse;
				ptCurrMouse.x = LOWORD(lParam);
				ptCurrMouse.y = HIWORD(lParam);

				float DeltaX = (float)ptCurrMouse.x - (float)ptPrevMouse.x;
				float DeltaY = (float)ptCurrMouse.y - (float)ptPrevMouse.y;

				CamRotAngle.y = (DeltaX / 100.0f);
				CamRotAngle.x = (DeltaY / 100.0f);
				CAMMANAGER->vec_pCam[0]->GetRefRot().y += CamRotAngle.y;
				CAMMANAGER->vec_pCam[0]->GetRefRot().x += CamRotAngle.x;

				ptPrevMouse = ptCurrMouse;
			}
			break;
	}
}