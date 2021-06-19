#include "stdafx.h"
#include "Texture.h"

using namespace DirectX;

Texture::Texture()
	:Srv(nullptr), Sampler(nullptr)
{
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = -FLT_MAX;
	SamplerDesc.MaxLOD = FLT_MAX;
}

Texture::~Texture()
{
	SAFE_RELEASE(Srv);
	SAFE_RELEASE(Sampler);
}

void Texture::CreateTexture(_tstring file, _tstring path)
{
	{
		size_t index = file.find_last_of(_T('\\'));
		file = file.substr(index + 1, file.length());
	}

	_tstring textureFile = path + file;

	HRESULT hr;

	size_t index = file.find_last_of(_T('.'));
	_tstring format = file.substr(index + 1, file.length());

	TexMetadata metaData;
	ScratchImage image;
	wstring temp = L"";
	temp.assign(textureFile.begin(), textureFile.end());

	if (format == _T("tga"))
	{
		hr = GetMetadataFromTGAFile(temp.c_str(), metaData);
		assert(SUCCEEDED(hr));
		hr = LoadFromTGAFile(temp.c_str(), &metaData, image);
		assert(SUCCEEDED(hr));
	}
	else if (format == _T("dds"))
	{
		hr = GetMetadataFromDDSFile(temp.c_str(), DDS_FLAGS_NONE, metaData);
		assert(SUCCEEDED(hr));
		hr = LoadFromDDSFile(temp.c_str(), DDS_FLAGS_NONE, &metaData, image);
		assert(SUCCEEDED(hr));
	}
	else if (format == _T("hdr"))
	{
		assert(false);
	}
	else
	{
		hr = GetMetadataFromWICFile(temp.c_str(), WIC_FLAGS_NONE, metaData);
		assert(SUCCEEDED(hr));
		hr = LoadFromWICFile(temp.c_str(), WIC_FLAGS_NONE, &metaData, image);
		assert(SUCCEEDED(hr));
	}

	if (Srv != nullptr)
	{
		SAFE_RELEASE(Srv);
	}
	hr = CreateShaderResourceView(g_Device, image.GetImages(), image.GetImageCount(), metaData, &Srv);
	CHECK(hr);
}

void Texture::CreateSampler()
{
	if (Sampler)
	{
		SAFE_RELEASE(Sampler);
	}
	HRESULT hr;
	hr = g_Device->CreateSamplerState(&SamplerDesc, &Sampler);
	CHECK(hr);
}

void Texture::Render(int slot)
{
	if(Srv)
	{
		g_DeviceContext->PSSetShaderResources(slot, 1, &Srv);
	}
	if (Sampler)
	{
		g_DeviceContext->PSSetSamplers(slot, 1, &Sampler);
	}
}