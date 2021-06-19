#include "stdafx.h"
#include "Cam.h"

Cam::Cam()
{
	_Eye = Vector3(0, 5, -5);
	_Look = Vector3(0, 0, 0);
	_Up = Vector3(0, 1, 0);

	V = new View(_Eye, _Look, _Up);
	Proj = new Perspective(g_D3dDesc.Width, g_D3dDesc.Height, zn, zf, fov);
	O = new Orthographic(g_D3dDesc.Width, g_D3dDesc.Height);
	VP = new Viewport(g_D3dDesc.Width, g_D3dDesc.Height, 0.0f, 0.0f);

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ViewBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ProjectionBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &OrthoProjectionBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vector4);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ViewPosBuffer);
		assert(SUCCEEDED(hr));
	}
}

Cam::~Cam()
{
	SAFE_RELEASE(ViewBuffer);
	SAFE_RELEASE(ProjectionBuffer);
	SAFE_RELEASE(OrthoProjectionBuffer);
	SAFE_DELETE(V);
	SAFE_DELETE(P);
	SAFE_DELETE(VP);
	SAFE_DELETE(O);
}

HRESULT Cam::create()
{
	__super::create();

	return S_OK;
}

void Cam::destroy()
{
	__super::destroy();
}

void Cam::update()
{
	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);

	D3DXMatrixScaling(&S, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, Rot.y, Rot.x, Rot.z);
	D3DXMatrixTranslation(&T, Pos.x, Pos.y, Pos.z);

	W = S * R * T;
	RT = R * T;

	if (Object::P)
	{
		W = W * (*Object::P);
		RT = RT * (*Object::P);
	}

	Vector3 tempLook, tempUp;
	D3DXVec3TransformCoord(&WorldEye, &_Eye, &RT);
	tempLook = GetWorldPos();
	D3DXVec3TransformNormal(&tempUp, &_Up, &RT);

	V->Set(WorldEye, tempLook, tempUp);
	Proj->Set(g_D3dDesc.Width, g_D3dDesc.Height, zn, zf, fov);
	O->Set(g_D3dDesc.Width, g_D3dDesc.Height);
	VP->Set(g_D3dDesc.Width, g_D3dDesc.Height);

	Forward = tempLook - WorldEye;
	D3DXVec3Normalize(&Forward, &Forward);

	Up = tempUp;
	D3DXVec3Normalize(&Up, &Up);

	D3DXVec3Cross(&Right, &Up, &Forward);
	D3DXVec3Normalize(&Right, &Right);
}

void Cam::render()
{
	__super::render();

	Matrix ViewMat;
	V->GetMatrix(&ViewMat);
	g_DeviceContext->UpdateSubresource(ViewBuffer, 0, NULL, &ViewMat, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(1, 1, &ViewBuffer);

	Matrix ProjMat;
	Proj->GetMatrix(&ProjMat);
	g_DeviceContext->UpdateSubresource(ProjectionBuffer, 0, NULL, &ProjMat, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(2, 1, &ProjectionBuffer);

	Matrix OrhoMat;
	O->GetMatrix(&OrhoMat);
	g_DeviceContext->UpdateSubresource(OrthoProjectionBuffer, 0, NULL, &OrhoMat, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(3, 1, &OrthoProjectionBuffer);

	Vector4 viewPos;
	viewPos.x = WorldEye.x;
	viewPos.y = WorldEye.y;
	viewPos.z = WorldEye.z;

	g_DeviceContext->UpdateSubresource(ViewPosBuffer, 0, NULL, &viewPos, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(2, 1, &ViewPosBuffer);

	VP->RSSetViewport();
}