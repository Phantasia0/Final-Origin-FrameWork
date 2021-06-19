#include "stdafx.h"
#include "MainGame.h"

POINT g_ptMouse = { 0,0 };
ID3D11Device* g_Device;
ID3D11DeviceContext* g_DeviceContext;
IDXGISwapChain* g_SwapChain;
D3DDesc g_D3dDesc;
MainGame* Mg;
bool isCreate;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	isCreate = false;
	g_D3dDesc.AppName = _T("D3D Game");
	g_D3dDesc.Instance = hInstance;
	g_D3dDesc.bVsync = true;
	g_D3dDesc.Handle = NULL;
	g_D3dDesc.Width = 800;
	g_D3dDesc.Height = 600;
	g_D3dDesc.bFullScreen = false;
	g_D3dDesc.Background = Color(0.5f, 0.5f, 0.5f, 1.0f);
	g_D3dDesc.FixFrame = 100;

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = g_D3dDesc.Instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = g_D3dDesc.AppName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (g_D3dDesc.bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)g_D3dDesc.Width;
		devMode.dmPelsHeight = (DWORD)g_D3dDesc.Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	g_D3dDesc.Handle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, g_D3dDesc.AppName.c_str()
		, g_D3dDesc.AppName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, g_D3dDesc.Instance
		, NULL
	);
	assert(g_D3dDesc.Handle != NULL);

	RECT rect = { 0, 0, (LONG)g_D3dDesc.Width, (LONG)g_D3dDesc.Height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)g_D3dDesc.Width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)g_D3dDesc.Height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	MoveWindow
	(
		g_D3dDesc.Handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);

	ShowWindow(g_D3dDesc.Handle, SW_SHOWNORMAL);
	SetForegroundWindow(g_D3dDesc.Handle);
	SetFocus(g_D3dDesc.Handle);
	ShowCursor(true);

	D3DMANAGER;
	Mg = new MainGame();

	if (FAILED(Mg->init()))
	{
		return false;
	}
	isCreate = true;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TIMEMANAGER->SetFrame(g_D3dDesc.FixFrame);
			Mg->update();
			Mg->PreRender();
			D3DMANAGER->SetRenderTarget();
			D3DMANAGER->Clear(g_D3dDesc.Background);
			WRITEMANAGER->GetDc()->BeginDraw();
			Mg->render();
			WRITEMANAGER->GetDc()->EndDraw();
			D3DMANAGER->Present();
		}
	}

	Mg->release();
	SAFE_DELETE(Mg);

	if (g_D3dDesc.bFullScreen == true)
		ChangeDisplaySettings(NULL, 0);
	D3DMANAGER->ReleaseSingleton();

	DestroyWindow(g_D3dDesc.Handle);
	UnregisterClass(g_D3dDesc.AppName.c_str(), g_D3dDesc.Instance);

	return static_cast<int> (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		g_D3dDesc.Width = (float)LOWORD(lParam);
		g_D3dDesc.Height = (float)HIWORD(lParam);

		if (isCreate)
			D3DMANAGER->ResizeScreen(g_D3dDesc.Width, g_D3dDesc.Height);

		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	if (isCreate)
	{
		CAMMANAGER->WndProc(hWnd, message, wParam, lParam);

		if (ImGui::WndProc((UINT*)hWnd, message, wParam, lParam))
		{
			return true;
		}
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}