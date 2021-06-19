#pragma once

class Texture
{
private:
	ID3D11SamplerState* Sampler;
	D3D11_SAMPLER_DESC SamplerDesc;
	ID3D11ShaderResourceView* Srv;
public:
	Texture();
	~Texture();
	void CreateTexture(_tstring file, _tstring path = _T(""));
	void CreateSampler();
	void Render(int slot = 0);
	MAKEREFGET(D3D11_SAMPLER_DESC, SamplerDesc)
	MAKEREFGET(ID3D11ShaderResourceView*, Srv)
};