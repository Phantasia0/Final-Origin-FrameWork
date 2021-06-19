#include "stdafx.h"
#include "Refraction.h"

Refraction::Refraction(_tstring normalfile, _tstring path)
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(CAM_DESC);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &VPBuffer);
		assert(SUCCEEDED(hr));
	}

	RefractMap = new Texture();
	RefractMap->GetRefSamplerDesc().Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	RefractMap->CreateSampler();

	PreTarget = new RenderTarget();
	PreTarget->CreateBackBuffer(g_D3dDesc.Width, g_D3dDesc.Height);
	D3DMANAGER->GetRefvecRT().emplace_back(PreTarget);

	MirrorCam = new Cam();
	MirrorCam->create();

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vector4);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &TimeBuffer);
		assert(SUCCEEDED(hr));
	}

	time.x = 0.0f;

	NormalMap = new Texture();
	NormalMap->CreateTexture(normalfile, path);
	NormalMap->GetRefSamplerDesc().Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	NormalMap->GetRefSamplerDesc().AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	NormalMap->GetRefSamplerDesc().AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	NormalMap->CreateSampler();
}

Refraction::~Refraction()
{
	MirrorCam->destroy();
	SAFE_DELETE(MirrorCam);
	SAFE_DELETE(PreTarget);
	SAFE_DELETE(RefractMap);
	SAFE_DELETE(NormalMap);
	SAFE_RELEASE(TimeBuffer);
	SAFE_RELEASE(VPBuffer);
}

void Refraction::render()
{
	RefractMap->GetRefSrv() = PreTarget->GetSRV();

	g_DeviceContext->UpdateSubresource(VPBuffer, 0, NULL, &view_desc, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(10, 1, &VPBuffer);

	RefractMap->Render(10);

	time.x += DeltaTime;
	g_DeviceContext->UpdateSubresource(TimeBuffer, 0, NULL, &time, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(10, 1, &TimeBuffer);
	NormalMap->Render(11);
}

void Refraction::PreRender()
{
	PreTarget->Set(Color(0.5f, 0.5f, 0.5f, 1));
	Vector3 minus = DestEye - SourEye;
	minus.z *= -1.0f;
	Vector3 look = DestEye + minus;

	D3DXMatrixLookAtLH(&view_desc.V, &DestEye, &look, &Up);
	D3DXMatrixOrthoLH(&view_desc.P, ClipSize.x, ClipSize.y, 0.1f, 1000.0f);

	MirrorCam->GetRefVP()->Set(g_D3dDesc.Width, g_D3dDesc.Height);
	MirrorCam->GetRefV()->Set(&view_desc.V);
	MirrorCam->GetRefProj()->Set(&view_desc.P);
	MirrorCam->render();
}