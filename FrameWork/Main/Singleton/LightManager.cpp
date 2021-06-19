#include "stdafx.h"
#include "LightManager.h"

LightManager::LightManager()
{
	dirLight.Dir = Vector3(0, -1, 0);

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(DirLight);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &DirLightBuffer);
		assert(SUCCEEDED(hr));
	}
}

LightManager::~LightManager()
{
	SAFE_RELEASE(DirLightBuffer);
}

void LightManager::render()
{
	g_DeviceContext->UpdateSubresource(DirLightBuffer, 0, NULL, &dirLight, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(4, 1, &DirLightBuffer);
}