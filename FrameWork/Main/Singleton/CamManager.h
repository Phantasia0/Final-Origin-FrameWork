#pragma once

class CamManager : public SingletonBase<CamManager>
{
private:
	POINT ptPrevMouse;
	bool isButtonDown = false;
	Vector3 CamRotAngle;
public:
	vector<Cam*> vec_pCam;
	CamManager();
	~CamManager();
	void AddCamList(Cam* p);
	void update();
	void render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};