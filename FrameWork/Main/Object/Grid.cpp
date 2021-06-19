#include "stdafx.h"
#include "Grid.h"

Grid::Grid(int x, int z, float scale, _tstring key, string input)
{
	vertexCount = (x + z) * 2 + 4;
	vertex = new VERTEX_PC[vertexCount];

	MaxX = x;
	MaxZ = z;
	Scale = scale;
	shader = SHADERMANAGER->AddShader(key, input);
}

Grid::~Grid()
{
	SAFE_DELETE_ARRAY(vertex);
}

HRESULT Grid::create()
{
	__super::create();

	float HalfX = MaxX * Scale * 0.5f;
	float HalfZ = MaxZ * Scale * 0.5f;

	for (int i = 0; i <= MaxX; i++)
	{
		vertex[i * 2].Position = Vector3(-HalfX, 0, -HalfZ + (Scale * i));
		vertex[i * 2].Color = Vector3(1.0, 1.0, 1.0);
		vertex[i * 2 + 1].Position = Vector3(HalfX, 0, -HalfZ + (Scale * i));
		vertex[i * 2 + 1].Color = Vector3(1.0, 1.0, 1.0);
	}

	for (int i = MaxX + 1; i < MaxX + MaxZ + 2; i++)
	{
		vertex[i * 2].Position = Vector3(-HalfX + (Scale * (i - MaxX - 1)), 0, -HalfZ);
		vertex[i * 2].Color = Vector3(1.0, 1.0, 1.0);
		vertex[i * 2 + 1].Position = Vector3(-HalfX + (Scale * (i - MaxX - 1)), 0, HalfZ);
		vertex[i * 2 + 1].Color = Vector3(1.0, 1.0, 1.0);
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PC) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertex;

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	return S_OK;
}

void Grid::destroy()
{
	__super::destroy();
	SAFE_RELEASE(vertexBuffer);
}

void Grid::update()
{
	__super::update();
}

void Grid::render()
{
	__super::render();
	shader->Render();

	UINT stride = sizeof(VERTEX_PC);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	g_DeviceContext->Draw(vertexCount, 0);

	renderState->AfterRender();
}