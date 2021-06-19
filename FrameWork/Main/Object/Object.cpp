#include "stdafx.h"
#include "Object.h"

Object::Object()
{
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	Pos.x = 0.0f;
	Pos.y = 0.0f;
	Pos.z = 0.0f;

	Scale.x = 1.0f;
	Scale.y = 1.0f;
	Scale.z = 1.0f;

	Rot.x = 0.0f;
	Rot.y = 0.0f;
	Rot.z = 0.0f;

	P = nullptr;

	Up = Vector3(0.0f, 1.0f, 0.0f);
	Right = Vector3(1.0f, 0.0f, 0.0f);
	Forward = Vector3(0.0f, 0.0f, 1.0f);

	{
		D3D11_BUFFER_DESC desc = { 0 };

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &WorldBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Color);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ColorBuffer);
		assert(SUCCEEDED(hr));
	}

	Mtl = new Material();
	renderState = new RenderState();
	renderState->CreateRasterizerState();
	renderState->CreateDepthStencil();
	renderState->CreateBlendState();
}

Object::~Object()
{
	SAFE_RELEASE(WorldBuffer);
	SAFE_DELETE(Mtl);
	SAFE_DELETE(renderState);
}

HRESULT Object::create()
{
	D3DXMatrixIdentity(&W);
	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&RT);

	return S_OK;
}

void Object::destroy()
{
}

void Object::update()
{
	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);

	D3DXMatrixScaling(&S, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, Rot.y, Rot.x, Rot.z);
	D3DXMatrixTranslation(&T, Pos.x, Pos.y, Pos.z);

	W = S * R * T;
	RT = R * T;

	if (P)
	{
		W = W * (*P);
		RT = RT * (*P);
	}

	Right = Vector3(RT._11, RT._12, RT._13);
	Up = Vector3(RT._21, RT._22, RT._23);
	Forward = Vector3(RT._31, RT._32, RT._33);
}

void Object::render()
{
	g_DeviceContext->UpdateSubresource(WorldBuffer, 0, NULL, &W, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(0, 1, &WorldBuffer);
	g_DeviceContext->UpdateSubresource(ColorBuffer, 0, NULL, &color, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(0, 1, &ColorBuffer);
	Mtl->render();
	renderState->BeforeRender();
}