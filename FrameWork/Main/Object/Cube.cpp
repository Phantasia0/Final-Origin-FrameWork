#include "stdafx.h"
#include "Cube.h"

Cube::Cube(_tstring key, string input)
{
	vertex = new VERTEX_PCN[36];
	shader = SHADERMANAGER->AddShader(key, input);
}

Cube::~Cube()
{
	SAFE_DELETE_ARRAY(vertex);
}

HRESULT Cube::create()
{
	__super::create();

	Vector3 tempColor;
	tempColor = Vector3(1, 1, 1);

	vertex[0].Position = Vector3(-0.5f, -0.5f, -0.5f);
	vertex[1].Position = Vector3(-0.5f, 0.5f, -0.5f);
	vertex[2].Position = Vector3(0.5f, 0.5f, -0.5f);

	vertex[3].Position = Vector3(-0.5f, -0.5f, -0.5f);
	vertex[4].Position = Vector3(0.5f, 0.5f, -0.5f);
	vertex[5].Position = Vector3(0.5f, -0.5f, -0.5f);

	vertex[6].Position = Vector3(-0.5f, -0.5f, 0.5f);
	vertex[7].Position = Vector3(0.5f, 0.5f, 0.5f);
	vertex[8].Position = Vector3(-0.5f, 0.5f, 0.5f);

	vertex[9].Position = Vector3(-0.5f, -0.5f, 0.5f);
	vertex[10].Position = Vector3(0.5f, -0.5f, 0.5f);
	vertex[11].Position = Vector3(0.5f, 0.5f, 0.5f);

	vertex[12].Position = Vector3(-0.5f, -0.5f, 0.5f);
	vertex[13].Position = Vector3(-0.5f, 0.5f, 0.5f);
	vertex[14].Position = Vector3(-0.5f, 0.5f, -0.5f);

	vertex[15].Position = Vector3(-0.5f, -0.5f, 0.5f);
	vertex[16].Position = Vector3(-0.5f, 0.5f, -0.5f);
	vertex[17].Position = Vector3(-0.5f, -0.5f, -0.5f);

	vertex[18].Position = Vector3(0.5f, -0.5f, -0.5f);
	vertex[19].Position = Vector3(0.5f, 0.5f, -0.5f);
	vertex[20].Position = Vector3(0.5f, 0.5f, 0.5f);

	vertex[21].Position = Vector3(0.5f, -0.5f, -0.5f);
	vertex[22].Position = Vector3(0.5f, 0.5f, 0.5f);
	vertex[23].Position = Vector3(0.5f, -0.5f, 0.5f);

	vertex[24].Position = Vector3(-0.5f, 0.5f, -0.5f);
	vertex[25].Position = Vector3(-0.5f, 0.5f, 0.5f);
	vertex[26].Position = Vector3(0.5f, 0.5f, 0.5f);

	vertex[27].Position = Vector3(-0.5f, 0.5f, -0.5f);
	vertex[28].Position = Vector3(0.5f, 0.5f, 0.5f);
	vertex[29].Position = Vector3(0.5f, 0.5f, -0.5f);

	vertex[30].Position = Vector3(-0.5f, -0.5f, 0.5f);
	vertex[31].Position = Vector3(-0.5f, -0.5f, -0.5f);
	vertex[32].Position = Vector3(0.5f, -0.5f, -0.5f);

	vertex[33].Position = Vector3(-0.5f, -0.5f, 0.5f);
	vertex[34].Position = Vector3(0.5f, -0.5f, -0.5f);
	vertex[35].Position = Vector3(0.5f, -0.5f, 0.5f);

	Vector3 v1, v2, n;
	for (int i = 0; i < 36; i++)
	{
		v1 = vertex[i + 1].Position - vertex[i].Position;
		v2 = vertex[i + 2].Position - vertex[i + 1].Position;

		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		vertex[i].Normal = n;
		vertex[i].Color = tempColor;
		vertex[i+1].Normal = n;
		vertex[i+1].Color = tempColor;
		vertex[i+2].Normal = n;
		vertex[i+2].Color = tempColor;
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PCN) * 36;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertex;

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	return S_OK;
}

void Cube::destroy()
{
	__super::destroy();
	SAFE_RELEASE(vertexBuffer);
}

void Cube::update()
{
	__super::update();
	COL_CUBE temp;
	D3DXVec3TransformCoord(&col.min, &temp.min, &W);
	D3DXVec3TransformCoord(&col.max, &temp.max, &W);
}

void Cube::render()
{
	__super::render();
	shader->Render();

	UINT stride = sizeof(VERTEX_PCN);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_DeviceContext->Draw(36, 0);

	renderState->AfterRender();
}