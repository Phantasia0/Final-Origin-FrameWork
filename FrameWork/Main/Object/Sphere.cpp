#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(UINT slice, UINT stack, _tstring key, string input)
{
	this->slice = slice;
	this->stack = stack;

	shader = SHADERMANAGER->AddShader(key, input);
}

Sphere::~Sphere()
{
}

HRESULT Sphere::create()
{
	float phiStep = PI * 2.0f / stack;
	float thetaStep = PI * 2.0f / slice;

	vector<VERTEX_PCN> v;
	vector<VERTEX_PCN>::iterator iter;

	for (UINT i = 0; i < v.size(); i++)
	{
		vertex[i] = v[i];
	}

	VERTEX_PCN temp;
	temp.Position = Vector3(0, 1, 1);
	temp.Color = Vector3(1, 1, 1);
	temp.Normal = temp.Position;
	v.emplace_back(temp);

	for (UINT i = 0; i <= stack - 1; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= slice; j++)
		{
			float theta = j * thetaStep;

			D3DXVECTOR3 p = D3DXVECTOR3
			(
				(sinf(phi) * cosf(theta)),
				(cosf(phi)),
				(sinf(phi) * sinf(theta))
			);

			temp.Position = Vector3(p.x, p.y, p.z);
			temp.Normal = temp.Position;
			v.emplace_back(temp);
		}
	}

	temp.Position = Vector3(0, -1, 0);
	temp.Normal = temp.Position;
	v.emplace_back(temp);

	vertexCount = v.size();
	vertex = new VERTEX_PCN[vertexCount];

	copy(v.begin(), v.end(), stdext::checked_array_iterator<VERTEX_PCN*>(vertex, vertexCount));
	v.clear();
	v.shrink_to_fit();

	vector<UINT> vindices;
	for (UINT i = 1; i <= slice; i++)
	{
		vindices.emplace_back(0);
		vindices.emplace_back(i + 1);
		vindices.emplace_back(i);
	}

	UINT baseIndex = 1;
	UINT ringVertexCount = slice + 1;
	for (UINT i = 0; i < stack - 2; i++)
	{
		for (UINT j = 0; j < slice; j++)
		{
			vindices.emplace_back(baseIndex + i * ringVertexCount + j);
			vindices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
			vindices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);

			vindices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
			vindices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
			vindices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	UINT southPoleIndex = v.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < slice; i++)
	{
		vindices.emplace_back(southPoleIndex);
		vindices.emplace_back(baseIndex + i);
		vindices.emplace_back(baseIndex + i + 1);
	}

	index = new UINT[vindices.size()];
	indexCount = vindices.size();
	copy(vindices.begin(), vindices.end(), stdext::checked_array_iterator<UINT *>(index, indexCount));

	vindices.clear();
	vindices.shrink_to_fit();

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PCN) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertex;

		CHECK(g_Device->CreateBuffer(&desc, &data, &vertexBuffer));
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = index;

		CHECK(g_Device->CreateBuffer(&desc, &data, &IndexBuffer));
	}

	return S_OK;
}

void Sphere::destroy()
{
	__super::destroy();
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(IndexBuffer);
}

void Sphere::update()
{
	__super::update();
}

void Sphere::render()
{
	__super::render();
	shader->Render();

	UINT stride = sizeof(VERTEX_PCN);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_DeviceContext->DrawIndexed(indexCount, 0, 0);

	renderState->AfterRender();
}