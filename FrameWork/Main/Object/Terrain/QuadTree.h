#pragma once

class QuadTree
{
private:
	struct Node
	{
		Node* Child[4] = { nullptr };
		int Level;
		UINT* Indices;
		ID3D11Buffer* IndexBuffer;
		VERTEX_PTN* vertex[4];
		Vector3 Center;
		float Radius;

		Node()
		{
			Indices = nullptr;
			IndexBuffer = nullptr;
		}

		~Node()
		{
			SAFE_RELEASE(IndexBuffer);
			if (Indices)
			{
				delete[] Indices;
			}
		}

		void MakeCenter(float scale)
		{
			Center = Vector3(0.0f, 0.0f, 0.0f);
			float min = 0.0f;
			float max = 0.0f;

			for (UINT i = 0; i < 4; i++)
			{
				Center += vertex[i]->Position;

				if (min > vertex[i]->Position.y)
				{
					min = vertex[i]->Position.y;
				}
				if (max < vertex[i]->Position.y)
				{
					max = vertex[i]->Position.y;
				}
			}
			Center /= 4.0f;
			float heightscale = (max - min) * 0.5f;
			Radius = (heightscale > scale) ? heightscale : scale;
		}
	};

	void AddChildren(Node* parent, LONG Level, POINT min);
	void RenderNode(Node* parent);
	void DeleteNode(Node* parent);

	Node* Root;
	LONG Depth;
	LONG MaxIndex;
	Frustum* frustum = nullptr;
	LONG** index;
public:
	Terrain* terrain;
	QuadTree();
	~QuadTree();
	HRESULT create();
	void destroy();
	void update();
	void render();
};