#include "stdafx.h"
#include "Dijkstra.h"

void DiNode::Reset()
{
	F = FLT_MAX;
	P = nullptr;
	isFind = false;
}

void Dijkstra(vector<DiNode>& Nodes, vector<DiNode*>& Way, int Start, int End)
{
	priority_queue<PNode, vector<PNode>, Nodecompare> Que;

	for (int i = 0; i < Nodes.size(); i++)
	{
		Nodes[i].Reset();
	}

	DiNode* pTemp = &Nodes[Start];
	pTemp->F = 0.0f;
	Que.push({ pTemp, pTemp->F });

	while (!Que.empty())
	{
		PNode Temp = Que.top();
		Temp.first->isFind = true;
		Que.pop();

		if (Temp.first->Id == End)
		{
			break;
		}

		for (int i = 0; i < Temp.first->LinkList.size(); i++)
		{
			DiNode* loop = &Nodes[Temp.first->LinkList[i].first];

			if (loop->F > Temp.first->F + Temp.first->LinkList[i].second)
			{
				loop->F = Temp.first->F + Temp.first->LinkList[i].second;
				loop->P = Temp.first;

				if (!loop->isFind)
				{
					Que.push({ loop,loop->F });
				}
			}
		}
	}

	DiNode* p = &Nodes[End];
	Way.push_back(p);

	while (1)
	{
		p = p->P;

		if (p == nullptr)
		{
			break;
		}

		Way.push_back(p);
	}
}