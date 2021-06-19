#include "stdafx.h"
#include "QuadTree.h"

QuadTree::QuadTree()
{
}

QuadTree::~QuadTree()
{
}

void QuadTree::AddChildren(Node * parent, LONG Level, POINT min)
{
	if (Level > Depth) return;

	LONG Max = MaxIndex / pow(2L, Level);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			LONG Minx = min.x + Max * j;
			LONG Maxx = min.x + Max * (j + 1);
			LONG Minz = min.y + Max * i;
			LONG Maxz = min.y + Max * (i + 1);

			parent->Child[i * 2 + j] = new Node();
			Node* child = parent->Child[i * 2 + j];
			child->Level = Level;
			
			LONG Index[4];

			Index[0] = index[Minz][Minx];
			Index[1] = index[Minz][Maxx];
			Index[2] = index[Maxz][Minx];
			Index[3] = index[Maxz][Maxx];

			child->vertex[0] =
				&terrain->vertex[(int)Index[0]];
			child->vertex[1] =
				&terrain->vertex[(int)Index[1]];
			child->vertex[2] =
				&terrain->vertex[(int)Index[2]];
			child->vertex[3] =
				&terrain->vertex[(int)Index[3]];

			child->MakeCenter(terrain->ScaleX*(Max*0.5f + 1.0f));

			if (Max == 32)
			{
				child->Indices = new UINT[6144];
				vector<UINT> vecIndex;
				
				for (int z = Minz; z < Maxz; z++)
				{
					for (int x = Minx; x < Maxx; x++)
					{
						UINT Index = (z * terrain->MaxX) + x;
						vecIndex.emplace_back((UINT)Index);
						vecIndex.emplace_back((UINT)(Index + 1));
						vecIndex.emplace_back((UINT)(Index + terrain->MaxX + 1));

						vecIndex.emplace_back((UINT)Index);
						vecIndex.emplace_back((UINT)(Index + terrain->MaxX + 1));
						vecIndex.emplace_back((UINT)(Index + terrain->MaxX));
					}
				}

				copy(vecIndex.begin(), vecIndex.end(),
					stdext::checked_array_iterator<UINT*>(child->Indices, 6144));

				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
				desc.ByteWidth = sizeof(UINT) * 6144;
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

				D3D11_SUBRESOURCE_DATA data = { 0 };
				data.pSysMem = child->Indices;
				CHECK(g_Device->CreateBuffer(&desc, &data, &child->IndexBuffer));
			}

			AddChildren(parent->Child[i * 2 + j], Level + 1, POINT{ Minx, Minz });
		}
	}
}

void QuadTree::RenderNode(Node * parent)
{
	if (!frustum->ContainSphere(parent->Center, parent->Radius))
	{
		return;
	}

	if (parent->IndexBuffer)
	{
		g_DeviceContext->IASetIndexBuffer(parent->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		g_DeviceContext->DrawIndexed(6144, 0, 0);
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		RenderNode(parent->Child[i]);
	}
}

void QuadTree::DeleteNode(Node * parent)
{
	for (int i = 0; i < 4; i++)
	{
		if (parent->Child[i])
		{
			DeleteNode(parent->Child[i]);
		}
	}
	delete parent;
}


HRESULT QuadTree::create()
{
	MaxIndex = terrain->MaxX - 1;
	double temp = log10(MaxIndex) / log10(2);
	int temp2 = round(log10(MaxIndex) / log10(2));
	double temp3 = abs(temp - temp2);

	Depth = temp2;

	if (temp3 >= DBL_EPSILON || Depth < 6)
	{
		assert(0 && "QuadTreeFailed");
	}

	index = new LONG*[terrain->MaxX];

	for (int i = 0; i < terrain->MaxX; i++)
	{
		index[i] = new LONG[terrain->MaxX];
	}

	for (int i = 0; i < terrain->MaxX; i++)
	{
		for (int j = 0; j < terrain->MaxX; j++)
		{
			index[i][j] = i* terrain->MaxX + j;
		}
	}

	Root = new Node();
	Root->Level = 0;

	LONG Index[4];
	Index[0] = index[0][0];
	Index[1] = index[0][MaxIndex];
	Index[2] = index[MaxIndex][0];
	Index[3] = index[MaxIndex][MaxIndex];

	Root->vertex[0] = &terrain->vertex[(int)Index[0]];
	Root->vertex[1] = &terrain->vertex[(int)Index[1]];
	Root->vertex[2] = &terrain->vertex[(int)Index[2]];
	Root->vertex[3] = &terrain->vertex[(int)Index[3]];

	Root->MakeCenter(terrain->ScaleX * (MaxIndex * 0.5f + 1.0f));

	frustum = new Frustum();
	frustum->GetRefpCam() = CAMMANAGER->vec_pCam[0];

	AddChildren(Root, Root->Level + 1, POINT{ 0, 0 });

	return S_OK;
}

void QuadTree::destroy()
{
	for (int i = 0; i < terrain->MaxX; i++)
	{
		SAFE_DELETE_ARRAY(index[i]);
	}
	SAFE_DELETE_ARRAY(index);

	DeleteNode(Root);
	SAFE_DELETE(frustum);
}

void QuadTree::update()
{
	terrain->update();
	frustum->Update();
}

void QuadTree::render()
{
	terrain->render();
	RenderNode(Root);
}