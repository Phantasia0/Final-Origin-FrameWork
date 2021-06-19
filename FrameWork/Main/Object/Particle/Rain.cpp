#include "stdafx.h"
#include "Rain.h"

Rain::Rain(UINT particleCount, _tstring key, string input)
	:Particle(particleCount)
{
	shader = SHADERMANAGER->AddShader(key, input);
	shader->CreateGeometryShader(key);

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(RAIN_DESC);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &RainBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ViewBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Matrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ProjectionBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vector4);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ViewPosBuffer);
		assert(SUCCEEDED(hr));
	}

	texture->CreateSampler();
	reset();
}

Rain::~Rain()
{
	SAFE_RELEASE(RainBuffer);
	SAFE_RELEASE(ViewBuffer);
	SAFE_RELEASE(ViewPosBuffer);
	SAFE_RELEASE(ProjectionBuffer);
	SAFE_RELEASE(vertexBuffer);
}

HRESULT Rain::create()
{
	Particle::create();
	return S_OK;
}

void Rain::destroy()
{
	Particle::destroy();
}

void Rain::update()
{
	Particle::update();
	RainDesc.time += DeltaTime;

	ImGui::SliderFloat3("Velocity", RainDesc.velocity, -1000, 1000);
	ImGui::SliderFloat3("size", RainDesc.size, -500, 500);
	ImGui::ColorEdit4("Color", RainDesc.color);
	ImGui::SliderFloat3("Origin", RainDesc.origin, -50, 50);
	ImGui::SliderFloat3("width", size, 0, 1000);
	ImGui::SliderFloat4("minmax", minmax, 0, 8);
	if (ImGui::Button("reset"))
	{
		reset();
	}
}

void Rain::render()
{
	Particle::render();

	UINT stride = sizeof(VERTEX_PS);
	UINT offset = 0;

	g_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	g_DeviceContext->UpdateSubresource(RainBuffer, 0, NULL, &RainDesc, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(10, 1, &RainBuffer);

	shader->Render();
	texture->Render();

	Matrix ViewMat;
	CAMMANAGER->vec_pCam[0]->GetRefV()->GetMatrix(&ViewMat);
	g_DeviceContext->UpdateSubresource(ViewBuffer, 0, NULL, &ViewMat, 0, 0);
	g_DeviceContext->GSSetConstantBuffers(0, 1, &ViewBuffer);

	Matrix ProjMat;
	CAMMANAGER->vec_pCam[0]->GetRefProj()->GetMatrix(&ProjMat);
	g_DeviceContext->UpdateSubresource(ProjectionBuffer, 0, NULL, &ProjMat, 0, 0);
	g_DeviceContext->GSSetConstantBuffers(1, 1, &ProjectionBuffer);

	Vector4 viewPos;
	viewPos.x = CAMMANAGER->vec_pCam[0]->GetRefWorldEye().x;
	viewPos.y = CAMMANAGER->vec_pCam[0]->GetRefWorldEye().y;
	viewPos.z = CAMMANAGER->vec_pCam[0]->GetRefWorldEye().z;
	g_DeviceContext->UpdateSubresource(ViewPosBuffer, 0, NULL, &viewPos, 0, 0);
	g_DeviceContext->GSSetConstantBuffers(2, 1, &ViewPosBuffer);

	g_DeviceContext->GSSetShader(shader->GetRefGeometryShader(), 0, 0);
	g_DeviceContext->Draw(particleCount, 0);
	g_DeviceContext->GSSetShader(0, 0, 0);
	renderState->AfterRender();
}

void Rain::reset()
{
	vertices.clear();
	vertices.resize(particleCount);

	Vector2 scale;
	for (UINT i = 0; i < particleCount; i++)
	{
		scale.x = RndFloat(minmax.x, minmax.y);
		scale.y = RndFloat(minmax.z, minmax.w);

		Vector3 position;
		position.x = RndFloat(-size.x, size.x);
		position.y = RndFloat(-size.y, size.y);
		position.z = RndFloat(-size.z, size.z);

		vertices[i].Position = position;
		vertices[i].Size = scale;
	}
	SAFE_RELEASE(vertexBuffer);

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VERTEX_PS) * vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices.data();

		CHECK(g_Device->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	RainDesc.size = size;
}