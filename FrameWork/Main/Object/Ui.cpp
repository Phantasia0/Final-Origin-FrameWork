#include "stdafx.h"
#include "Ui.h"

bool Ui::Click(WPARAM keyCode)
{
	if (MouseOver())
	{
		if (KEYMANAGER->isKeyDown(keyCode))
		{
			if (click)
			{
				click();
			}
			return true;
		}
	}

	return false;
}

bool Ui::Drag(WPARAM keyCode)
{
	if (MouseOver())
	{
		if (KEYMANAGER->StayKeyDown(keyCode))
		{
			if (drag)
			{
				drag();
			}
			return true;
		}
	}
	return false;
}

bool Ui::MouseOver()
{
	if (!isActive) return false;
	if (PtInRect(this, g_ptMouse))
	{
		if (mouseOver)
		{
			mouseOver();
		}
		return true;
	}

	return false;
}

Ui::Ui(_tstring key, string input)
	:texture(nullptr)
{
	click = NULL;
	mouseOver = NULL;
	drag = NULL;

	shader = SHADERMANAGER->AddShader(key, input);
	vertex = new VERTEX_PTN[6];
	texture = new Texture();

	renderState->GetRefDepthStencilDesc().DepthEnable = false;
	renderState->CreateDepthStencil();

	isActive = true;
}

Ui::~Ui()
{
}

HRESULT Ui::create(float minU, float minV, float maxU, float maxV)
{
	__super::create();

	vertex[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertex[1].Position = Vector3(-0.5f, 0.5f, 0.0f);
	vertex[2].Position = Vector3(0.5f, -0.5f, 0.0f);

	vertex[3].Position = Vector3(-0.5f, 0.5f, 0.0f);
	vertex[4].Position = Vector3(0.5f, 0.5f, 0.0f);
	vertex[5].Position = Vector3(0.5f, -0.5f, 0.0f);

	vertex[0].Uv = Vector2(0.0f, 1.0f);
	vertex[1].Uv = Vector2(0.0f, 0.0f);
	vertex[2].Uv = Vector2(1.0f, 1.0f);

	vertex[3].Uv = Vector2(0.0f, 0.0f);
	vertex[4].Uv = Vector2(1.0f, 0.0f);
	vertex[5].Uv = Vector2(1.0f, 1.0f);

	UV.MinUV = Vector2(minU, minV);
	UV.MaxUV = Vector2(maxU, maxV);

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(cUV);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &UVBuffer);
		assert(SUCCEEDED(hr));
	}

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

void Ui::destroy()
{
	__super::destroy();
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(UVBuffer);
	SAFE_DELETE(texture);

	for (UINT i = 0; i < vecChild.size(); i++)
	{
		vecChild[i].second->destroy();
		SAFE_DELETE(vecChild[i].second);
	}
	vecChild.clear();
	vecChild.shrink_to_fit();
}

void Ui::update()
{
	__super::update();

	for (UINT i = 0; i < vecChild.size(); i++)
	{
		vecChild[i].second->update();
	}
}

void Ui::render()
{
	if (!isActive) return;

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

	if (MaxFrameX != 1 || MaxFrameY != 1)
	{
		UV.MinUV.x = 1.0f / MaxFrameX * CurrentFrameX;
		UV.MaxUV.x = 1.0f / MaxFrameX * (CurrentFrameX + 1);
		UV.MinUV.y = 1.0f / MaxFrameY * CurrentFrameY;
		UV.MaxUV.y = 1.0f / MaxFrameY * (CurrentFrameY + 1);
	}

	g_DeviceContext->UpdateSubresource(UVBuffer, 0, NULL, &UV, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(4, 1, &UVBuffer);
	g_DeviceContext->Draw(6, 0);

	renderState->AfterRender();

	for (UINT i = 0; i < vecChild.size(); i++)
	{
		vecChild[i].second->render();
	}
}