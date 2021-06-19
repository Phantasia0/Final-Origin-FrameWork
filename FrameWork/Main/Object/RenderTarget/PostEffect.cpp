#include "stdafx.h"
#include "PostEffect.h"

PostEffect::PostEffect(_tstring key, string input)
{
	shader = SHADERMANAGER->AddShader(key, input);

	renderState = new RenderState();
	texture = new Texture();
	texture->CreateSampler();

	renderState->GetRefDepthStencilDesc().DepthEnable = false;
	renderState->CreateDepthStencil();

	vertices = new VERTEX_PT[6];
	vertices[0].Position = {-1.0f, -1.0f, 0.5f};
	vertices[1].Position = { -1.0f, 1.0f, 0.5f };
	vertices[2].Position = { 1.0f, -1.0f, 0.5f };
	vertices[3].Position = { 1.0f, -1.0f, 0.5f };
	vertices[4].Position = { -1.0f, 1.0f, 0.5f };
	vertices[5].Position = { 1.0f, 1.0f, 0.5f };

	vertices[0].Uv = { 0.0f, 1.0f };
	vertices[1].Uv = { 0.0f, 0.0f };
	vertices[2].Uv = { 1.0f, 1.0f };
	vertices[3].Uv = { 1.0f, 1.0f };
	vertices[4].Uv = { 0.0f, 0.0f };
	vertices[5].Uv = { 1.0f, 0.0f };

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PT) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
}

PostEffect::~PostEffect()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(texture);
	SAFE_DELETE(renderState);
	SAFE_RELEASE(vertexBuffer);
}

HRESULT PostEffect::create()
{
	__super::create();
	return S_OK;
}

void PostEffect::destroy()
{
	__super::destroy();
}

void PostEffect::update()
{
	__super::update();
}

void PostEffect::render()
{
	__super::render();
	shader->Render();
	if (texture)
	{
		texture->Render();
	}

	UINT stride = sizeof(VERTEX_PT);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Render();
	texture->Render();

	renderState->BeforeRender();
	g_DeviceContext->Draw(6, 0);
	renderState->AfterRender();
}