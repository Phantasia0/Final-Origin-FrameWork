#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height)
	:width(width), height(height)
{
	HRESULT hr;
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = this->width;
		desc.Height = this->height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		hr = g_Device->CreateTexture2D(&desc, nullptr, &rtvTexture);
		CHECK(hr);
	}

	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = g_Device->CreateRenderTargetView(rtvTexture, &desc, &rtv);
		CHECK(hr);
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		hr = g_Device->CreateShaderResourceView(rtvTexture, &desc, &srv);
		CHECK(hr);
	}

	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = this->width;
		desc.Height = this->height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

		hr = g_Device->CreateTexture2D(&desc, nullptr, &dsvTexture);
		CHECK(hr);
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		hr = g_Device->CreateDepthStencilView(dsvTexture, &desc, &dsv);
		CHECK(hr);
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		
		hr = g_Device->CreateShaderResourceView(dsvTexture, &desc, &srv2);
		CHECK(hr);
	}
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(rtvTexture);
	SAFE_RELEASE(dsvTexture);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(srv2);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(dsv);
}

void RenderTarget::Set(Color clear)
{
	D3DMANAGER->SetRenderTarget(rtv, dsv);
	D3DMANAGER->Clear(clear, rtv, dsv);
}

void RenderTarget::ResizeScreen(float width, float height)
{
	if (width < 1 || height < 1) return;

	this->width = width;
	this->height = height;

	DeleteBackBuffer();
	CreateBackBuffer(width, height);
}

void RenderTarget::CreateBackBuffer(float width, float height)
{
	HRESULT hr;
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		hr = g_Device->CreateTexture2D(&desc, nullptr, &rtvTexture);
		CHECK(hr);
	}

	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = g_Device->CreateRenderTargetView(rtvTexture, &desc, &rtv);
		CHECK(hr);
	}

	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = this->width;
		desc.Height = this->height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

		hr = g_Device->CreateTexture2D(&desc, nullptr, &dsvTexture);
		CHECK(hr);
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		hr = g_Device->CreateDepthStencilView(dsvTexture, &desc, &dsv);
		CHECK(hr);
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		hr = g_Device->CreateShaderResourceView(rtvTexture, &desc, &srv);
		CHECK(hr);
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		hr = g_Device->CreateShaderResourceView(dsvTexture, &desc, &srv2);
		CHECK(hr);
	}
}

void RenderTarget::DeleteBackBuffer()
{
	SAFE_RELEASE(rtvTexture);
	SAFE_RELEASE(dsvTexture);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(srv2);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(dsv);
}