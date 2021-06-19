#pragma once

class RenderTarget;

struct D3DDesc
{
	_tstring AppName;
	HINSTANCE Instance;
	HWND Handle;
	float Width;
	float Height;
	bool bVsync;
	D3DXCOLOR Background;
	int FixFrame;
	bool bFullScreen;
};

class D3D : public SingletonBase<D3D>
{
private:
	ID3D11Texture2D* backBuffer = nullptr;

	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;

	void CreateSwapChainAndDevice();
	void CreateBackBuffer(float width, float height);
	void DeleteBackBuffer();

	vector<RenderTarget*> vecRT;
public:
	D3D();
	~D3D();

	void SetRenderTarget(ID3D11RenderTargetView* rtv = NULL, ID3D11DepthStencilView* dsv = NULL);
	void Clear(D3DXCOLOR color = D3DXCOLOR(0xFF555566), ID3D11RenderTargetView* rtv = NULL, ID3D11DepthStencilView* dsv = NULL);
	void Present();
	void ResizeScreen(float width, float height);

	MAKEREFGET(vector<RenderTarget*>, vecRT)
};