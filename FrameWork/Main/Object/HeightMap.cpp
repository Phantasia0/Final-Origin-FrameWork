#include "stdafx.h"
#include "HeightMap.h"

HeightMap::HeightMap(_tstring key, string input)
{
	shader = SHADERMANAGER->AddShader(key, input);
	texture = new Texture();
}

HeightMap::~HeightMap()
{
	SAFE_DELETE(texture);
}

void HeightMap::create(_tstring file, _tstring path, float Heightscale, float scale)
{
	__super::create();

	_tstring Raw = path + file;
	
	FILE* fp;
	fopen_s(&fp, Raw.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	unsigned int Size = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* Height = new BYTE[Size];
	unsigned long long i = 0;
	while (!feof(fp))
	{
		char c = fgetc(fp);
		Height[i] = c;
		i++;
	}

	fclose(fp);

	MaxX = (int)sqrtf((float)Size);
	MaxZ = MaxX;

	vector<Vector3> vecP;
	vector<Vector2> vecT;

	Scale = scale;
	float half = MaxZ * 0.5f * scale;

	for (int z = 0; z < MaxZ; z++)
	{
		for (int x = 0; x < MaxZ; x++)
		{
			int Index = (z * MaxZ) + x;

			float _x = x * scale - half;
			float _z = -z * scale + half;
			float _y = (float)Height[Index] / 127.0f * MaxZ * scale * Heightscale;

			vecP.emplace_back(Vector3(_x, _y, _z));

			float _u = float(x) / float(MaxZ - 1);
			float _v = float(z) / float(MaxZ - 1);

			vecT.emplace_back(Vector2(_u, _v));
		}
	}

	VERTEX_PTN temp;
	vector<VERTEX_PTN> VecVertex;

	for (int z = 0; z < MaxZ - 1; z++)
	{
		for (int x = 0; x < MaxZ - 1; x++)
		{
			Vector3 v1, v2, v3, cross;
			int Index = (z * MaxZ) + x;
			
			v1 = vecP[Index];
			v2 = vecP[Index + 1];
			v3 = vecP[Index + MaxZ + 1];
			D3DXVec3Cross(&cross, &(v2 - v1), &(v3 - v2));

			temp.Uv = vecT[Index];
			temp.Position = v1;
			temp.Normal = cross;
			VecVertex.emplace_back(temp);

			temp.Uv = vecT[Index + 1];
			temp.Position = v2;
			VecVertex.emplace_back(temp);

			temp.Uv = vecT[Index + MaxZ + 1];
			temp.Position = v3;
			VecVertex.emplace_back(temp);

			v1 = vecP[Index];
			v2 = vecP[Index + MaxZ + 1];
			v3 = vecP[Index + MaxZ];
			D3DXVec3Cross(&cross, &(v2 - v1), &(v3 - v2));

			temp.Uv = vecT[Index];
			temp.Position = v1;
			temp.Normal = cross;
			VecVertex.emplace_back(temp);

			temp.Uv = vecT[Index + MaxZ + 1];
			temp.Position = v2;
			VecVertex.emplace_back(temp);

			temp.Uv = vecT[Index + MaxZ];
			temp.Position = v3;
			VecVertex.emplace_back(temp);
		}
	}

	vertexCount = VecVertex.size();
	vertex = new VERTEX_PTN[vertexCount];

	copy(VecVertex.begin(), VecVertex.end(), stdext::checked_array_iterator<VERTEX_PTN*>(vertex, vertexCount));

	VecVertex.clear();
	VecVertex.shrink_to_fit();
	vecP.clear();
	vecP.shrink_to_fit();
	vecT.clear();
	vecT.shrink_to_fit();

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PTN) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertex;

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
}

void HeightMap::destroy()
{
	__super::create();
	SAFE_DELETE_ARRAY(vertex);
}

void HeightMap::update()
{
	__super::update();
}

void HeightMap::render()
{
	__super::render();
	shader->Render();
	texture->Render();
	Mtl->render();

	UINT stride = sizeof(VERTEX_PTN);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_DeviceContext->Draw(vertexCount, 0);
	
	renderState->AfterRender();
}