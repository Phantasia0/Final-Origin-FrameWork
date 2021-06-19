#pragma once

class RenderTarget
{
private:
	UINT width, height;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* srv2;
	ID3D11DepthStencilView* dsv;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* rtvTexture;
	ID3D11Texture2D* dsvTexture;
public:
	RenderTarget(UINT width = g_D3dDesc.Width, UINT height = g_D3dDesc.Height);
	~RenderTarget();
	void Set(Color clear = Color(1, 1, 1, 1));
	ID3D11ShaderResourceView* GetSRV() { return srv; }
	ID3D11ShaderResourceView* GetSRV2() { return srv2; }
	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11DepthStencilView* GetDSV() { return dsv; }
	void ResizeScreen(float width, float height);
	void CreateBackBuffer(float width, float height);
	void DeleteBackBuffer();
};