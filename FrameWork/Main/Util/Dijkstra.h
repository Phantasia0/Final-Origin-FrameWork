#pragma once

using Link = pair<unsigned int, float>;

class DiNode
{
public:
	Vector3 Pos;

	float F;
	unsigned int Id;
	DiNode* P;
	bool isFind;
	vector<Link> LinkList;

	void Reset();
};

using PNode = pair<DiNode*, float>;

struct Nodecompare
{
	bool operator()(PNode& a, PNode& b)
	{
		return a.second > b.second;
	}
};

void Dijkstra(vector<DiNode>& Nodes, _Inout_ vector<DiNode*>& Way, int Start, int End);