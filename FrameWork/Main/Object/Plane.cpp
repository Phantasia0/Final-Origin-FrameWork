#include "stdafx.h"
#include "Plane.h"

Plane::Plane(_tstring key, string input)
	:texture(nullptr)
{
	vertex = new VERTEX_PTN[6];
	shader = SHADERMANAGER->AddShader(key, input);
	texture = new Texture();
}

Plane::~Plane()
{
}

HRESULT Plane::create(float minU, float minV, float maxU, float maxV)
{
	__super::create();

	vertex[0].Uv = Vector2(minU, maxV);
	vertex[1].Uv = Vector2(minU, minV);
	vertex[2].Uv = Vector2(maxU, maxV);
	vertex[3].Uv = Vector2(maxU, maxV);
	vertex[4].Uv = Vector2(minU, minV);
	vertex[5].Uv = Vector2(maxU, minV);

	float halfX = 0.5f;
	float halfZ = 0.5f;

	vertex[0].Position = Vector3(-halfX, 0.0f, -halfZ);
	vertex[1].Position = Vector3(-halfX, 0.0f, halfZ);
	vertex[2].Position = Vector3(halfX, 0.0f, -halfZ);
	vertex[3].Position = Vector3(halfX, 0.0f, -halfZ);
	vertex[4].Position = Vector3(-halfX, 0.0f, halfZ);
	vertex[5].Position = Vector3(halfX, 0.0f, halfZ);

	vertex[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[4].Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[5].Normal = Vector3(0.0f, 1.0f, 0.0f);

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PTN) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertex;

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	return S_OK;
}

void Plane::destroy()
{
	__super::destroy();
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE(texture);
}

void Plane::update()
{
	__super::update();
}

void Plane::render()
{
	__super::render();
	shader->Render();

	if (texture)
	{
		texture->Render();
	}

	UINT stride = sizeof(VERTEX_PTN);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_DeviceContext->Draw(6, 0);

	renderState->AfterRender();
}