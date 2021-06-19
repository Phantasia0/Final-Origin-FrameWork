#include "stdafx.h"
#include "Terrain.h"

void Terrain::CreateStrutedBuffer()
{
	InputArray = new InputDesc[size];

	for (UINT i = 0; i < size; i++)
	{
		UINT index0 = index[i * 3 + 0];
		UINT index1 = index[i * 3 + 1];
		UINT index2 = index[i * 3 + 2];

		InputArray[i].v0 = vertex[index0].Position;
		InputArray[i].v1 = vertex[index1].Position;
		InputArray[i].v2 = vertex[index2].Position;

		InputArray[i].index = i;
	}

	OutputArray = new OutputDesc[size];

	{
		ID3D11Buffer* buffer;
		
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(InputDesc) * size;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(InputDesc);

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = InputArray;

		g_Device->CreateBuffer(&desc, &initData, &buffer);
		input = (ID3D11Resource*)buffer;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.NumElements = size;

		g_Device->CreateShaderResourceView(buffer, &srvDesc, &srv);
	}


	{
		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(OutputDesc) * size;
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(OutputDesc);

		g_Device->CreateBuffer(&desc, nullptr, &buffer);
		output = (ID3D11Resource*)buffer;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = size;

		g_Device->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
	}

	{
		ID3D11Buffer* buffer;
		D3D11_BUFFER_DESC desc = {};
		((ID3D11Buffer*)output)->GetDesc(&desc);
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		g_Device->CreateBuffer(&desc, nullptr, &buffer);

		result = (ID3D11Resource*)buffer;
	}
}

void Terrain::DeleteStrutedBuffer()
{
	SAFE_DELETE_ARRAY(InputArray);
	SAFE_RELEASE(input);
	SAFE_RELEASE(srv);
					 
	SAFE_DELETE_ARRAY(OutputArray);
	SAFE_RELEASE(output);
	SAFE_RELEASE(uav);
					 
	SAFE_RELEASE(result);
}

void Terrain::DeleteVertex()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);
}

void Terrain::CreateVertex()
{
	vector<VERTEX_PTN> VecVertex;
	float half = MaxX * 0.5f * ScaleX;

	for (int z = 0; z < MaxX; z++)
	{
		for (int x = 0; x < MaxX; x++)
		{
			int Index = (z * MaxX) + x;
			float _x = x * ScaleX - half;
			float _z = -z * ScaleX + half;
			float _y = Height[Index];

			float _u = float(x) / float(MaxX - 1);
			float _v = float(z) / float(MaxX - 1);
			VERTEX_PTN temp;
			temp.Normal = Vector3(0, 1, 0);
			temp.Uv = Vector2(_u, _v);
			temp.Position = Vector3(_x, _y, _z);
			VecVertex.push_back(temp);
		}
	}

	vertexCount = VecVertex.size();
	size = (MaxX - 1) * (MaxX - 1) * 2;

	vertex = new VERTEX_PTN[vertexCount];
	vector<UINT> Vecindex;

	for (int z = 0; z < MaxX - 1; z++)
	{
		for (int x = 0; x < MaxX - 1; x++)
		{
			UINT Index = z * MaxX + x;

			Vecindex.emplace_back(Index);
			Vecindex.emplace_back(Index + 1);
			Vecindex.emplace_back(Index + MaxX + 1);

			Vecindex.emplace_back(Index);
			Vecindex.emplace_back(Index + MaxX + 1);
			Vecindex.emplace_back(Index + MaxX);
		}
	}

	indexCount = Vecindex.size();
	index = new UINT[indexCount];

	copy(VecVertex.begin(), VecVertex.end(),
		stdext::checked_array_iterator<VERTEX_PTN*>(vertex, vertexCount));
	copy(Vecindex.begin(), Vecindex.end(),
		stdext::checked_array_iterator<UINT*>(index, indexCount));

	VecVertex.clear();
	VecVertex.shrink_to_fit();
	Vecindex.clear();
	Vecindex.shrink_to_fit();

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

Terrain::Terrain(_tstring key, string input)
{
	Height = new float[MaxX * MaxX];
	ZeroMemory(Height, sizeof(float)*MaxX*MaxX);

	shader = SHADERMANAGER->AddShader(key, input);

	texture = new Texture();
	compute = new Shader();
	compute->CreateComputeShader(_T("Shader/23.ComputeShader.hlsl"));

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(RayBuffer);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &rayBuffer);
		assert(SUCCEEDED(hr));
	}
}

Terrain::~Terrain()
{
	SAFE_DELETE(texture);
	SAFE_RELEASE(rayBuffer);
	SAFE_DELETE(compute);
	SAFE_DELETE_ARRAY(Height);
}

HRESULT Terrain::create()
{
	__super::create();

	CreateVertex();
	CreateStrutedBuffer();

	return S_OK;
}

void Terrain::destroy()
{
	__super::destroy();
	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);
}

void Terrain::update()
{
	__super::update();
}

void Terrain::render()
{
	__super::render();
	shader->Render();
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VERTEX_PTN);
	UINT offset = 0;

	texture->Render();
	Mtl->render();

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

bool Terrain::ComPutePicking(Ray wRay, OUT Vector3 & pos)
{
	compute->CSSet();

	ray.position = wRay.Pos;
	ray.direction = wRay.Dir;
	ray.size = size;

	g_DeviceContext->UpdateSubresource(rayBuffer, 0, NULL, &ray, 0, 0);
	g_DeviceContext->CSSetConstantBuffers(0, 1, &rayBuffer);

	g_DeviceContext->CSSetShaderResources(0, 1, &srv);
	g_DeviceContext->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	UINT x = ceil((float)size / 1024.0f);
	g_DeviceContext->Dispatch(x, 1, 1);

	g_DeviceContext->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE subResource;

	g_DeviceContext->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	memcpy(OutputArray, subResource.pData, sizeof(OutputDesc)* size);
	g_DeviceContext->Unmap(result, 0);

	float minDistance = FLT_MAX;
	int minIndex = -1;
	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = OutputArray[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		pos = ray.position + ray.direction * minDistance;
		return true;
	}

	return false;
}

void Terrain::SaveTerrain(string file)
{
	ofstream fout;
	fout.open(file, ios::out);
	if (fout.is_open())
	{
		fout << MaxX << endl;
		fout << ScaleX << endl;

		for (int i = 0; i < vertexCount; i++)
		{
			fout << vertex[i].Position.y << endl;
		}

		fout.close();
	}
}

void Terrain::LoadTerrain(string file)
{
	ifstream fin;
	fin.open(file, ios::in);
	if (fin.is_open())
	{
		fin >> MaxX;
		fin >> ScaleX;
		SAFE_DELETE_ARRAY(Height);
		Height = new float[MaxX * MaxX];
		for (int i = 0; i < MaxX * MaxX; i++)
		{
			fin >> Height[i];
		}

		fin.close();
	}
}