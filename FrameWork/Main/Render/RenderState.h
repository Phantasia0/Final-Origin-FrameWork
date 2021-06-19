#pragma once

class RenderState
{
private:
	ID3D11BlendState* BlendState[2];
	D3D11_BLEND_DESC BlendDesc[2];

	ID3D11DepthStencilState* DepthStencilState;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;

	ID3D11RasterizerState* RasterizerState;
	D3D11_RASTERIZER_DESC RasterizerDesc;

public:
	RenderState();
	~RenderState();
	void CreateRasterizerState();
	void CreateBlendState();
	void CreateDepthStencil();
	void BeforeRender();
	void AfterRender();
	MAKEREFGET(D3D11_RASTERIZER_DESC, RasterizerDesc)
	MAKEREFGET(D3D11_DEPTH_STENCIL_DESC, DepthStencilDesc)
	D3D11_BLEND_DESC& GetRefBlendDesc(int idx) { return BlendDesc[idx]; }
};