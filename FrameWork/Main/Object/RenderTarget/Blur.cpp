#include "stdafx.h"
#include "Blur.h"

Blur::Blur(_tstring key, string input)
{
	shader = SHADERMANAGER->AddShader(key, input);

	renderState = new RenderState();
	texture = new Texture();

	texture->CreateSampler();

	renderState->GetRefDepthStencilDesc().DepthEnable = false;
	renderState->CreateDepthStencil();

	vertices = new VERTEX_PT[6];

	vertices[0].Position = { -1.0f, -1.0f, 0.5f };
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

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BlurDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &blurBuffer);
		assert(SUCCEEDED(hr));
	}
}

Blur::~Blur()
{
	SAFE_RELEASE(blurBuffer);
}

void Blur::render()
{
	g_DeviceContext->UpdateSubresource(blurBuffer, 0, NULL, &blurDesc, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(10, 1, &blurBuffer);

	PostEffect::render();
}