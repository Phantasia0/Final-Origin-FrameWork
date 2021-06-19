#include "stdafx.h"
#include "Material.h"

Material::Material()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Mtl);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &MaterialBuffer);
		assert(SUCCEEDED(hr));
	}
}

Material::~Material()
{
	SAFE_RELEASE(MaterialBuffer);
}

void Material::update()
{
}

void Material::render()
{
	g_DeviceContext->UpdateSubresource(MaterialBuffer, 0, NULL, &mtl, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(1, 1, &MaterialBuffer);
}