#include "stdafx.h"
#include "Shadow.h"

Shadow::Shadow()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(CAM_DESC);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &VPBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(SHADOW_DESC);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ShadowBuffer);
		assert(SUCCEEDED(hr));
	}

	DepthMap = new Texture();
	DepthMap->GetRefSamplerDesc().AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	DepthMap->GetRefSamplerDesc().AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	DepthMap->GetRefSamplerDesc().AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	DepthMap->GetRefSamplerDesc().Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	DepthMap->GetRefSamplerDesc().ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	DepthMap->CreateSampler();
}

Shadow::~Shadow()
{
	SAFE_RELEASE(VPBuffer);
	SAFE_RELEASE(ShadowBuffer);
	SAFE_DELETE(DepthMap);
}

void Shadow::render()
{
	g_DeviceContext->UpdateSubresource(VPBuffer, 0, NULL, &view_desc, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(10, 1, &VPBuffer);
	g_DeviceContext->UpdateSubresource(ShadowBuffer, 0, NULL, &shadow_desc, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(10, 1, &ShadowBuffer);

	DepthMap->Render(10);
}