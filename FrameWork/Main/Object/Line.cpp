#include "stdafx.h"
#include "Line.h"

Line::Line(_tstring key, string input)
{
	shader = SHADERMANAGER->AddShader(key, input);

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PC) * 2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertex;

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
}

Line::~Line()
{
}

HRESULT Line::create()
{
	__super::create();

	vertex[0].Position = Vector3(0, 0, 0);
	vertex[0].Color = Vector3(1, 1, 1);
	vertex[1].Position = Vector3(0, 0, 0);
	vertex[1].Color = Vector3(1, 1, 1);

	return S_OK;
}

void Line::destroy()
{
}

void Line::update()
{
	g_DeviceContext->UpdateSubresource(vertexBuffer, 0, NULL, vertex, 0, 0);
}

void Line::render()
{
	__super::render();
	shader->Render();

	UINT stride = sizeof(VERTEX_PC);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	g_DeviceContext->Draw(2, 0);

	renderState->AfterRender();
}