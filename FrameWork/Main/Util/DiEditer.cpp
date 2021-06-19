#include "stdafx.h"
#include "DiEditer.h"

DiEditer::DiEditer()
{
	Pick = INT_MAX;
	sp = new Sphere();
	sp->create();
	sp->GetRefcolor() = Color(1, 0, 1, 1);

	ln = new Line();
	ln->create();
}

DiEditer::~DiEditer()
{
	sp->destroy();
	ln->destroy();
	SAFE_DELETE(sp);
	SAFE_DELETE(ln);
}

void DiEditer::AddNode(Vector3 pos)
{
	DiNode temp;
	temp.Id = Nodes.size();
	temp.Pos = pos;
	Nodes.push_back(temp);
}

void DiEditer::PopNode()
{
	for (int i = 0; i < Nodes.size(); i++)
	{
		for (int j = 0; j < Nodes[i].LinkList.size(); j++)
		{
			if (Nodes[i].LinkList[j].first == Nodes.size() - 1)
			{
				Nodes[i].LinkList.erase(Nodes[i].LinkList.begin() + j);
				j--;
			}
		}
	}

	Nodes.pop_back();
}

void DiEditer::LinkNode(int id1, int id2)
{
	Vector3 Dis = Nodes[id1].Pos - Nodes[id2].Pos;

	Link temp1;
	temp1.first = id2;
	temp1.second = D3DXVec3Length(&Dis);

	Link temp2;
	temp2.first = id1;
	temp2.second = D3DXVec3Length(&Dis);

	Nodes[id1].LinkList.push_back(temp1);
	Nodes[id2].LinkList.push_back(temp2);
}

int DiEditer::PickNode(Vector3 pos)
{
	int id = NULL;
	float min = FLT_MAX;

	for (int i = 0; i < Nodes.size(); i++)
	{
		Vector3 Dis = Nodes[i].Pos - pos;
		float len = D3DXVec3Length(&Dis);

		if (min > len)
		{
			min = len;
			id = i;
		}
	}

	return id;
}

void DiEditer::render()
{
	for (int i = 0; i < Nodes.size(); i++)
	{
		if (Pick == i)
		{
			sp->GetRefcolor() = Color(1, 0, 0, 1);
		}
		else
		{
			sp->GetRefcolor() = Color(1, 1, 1, 1);
		}

		sp->GetRefPos() = Nodes[i].Pos;
		sp->update();
		sp->render();

		for (int j = 0; j < Nodes[i].LinkList.size(); j++)
		{
			ln->vertex[0].Position = Nodes[i].Pos;
			ln->vertex[1].Position = Nodes[Nodes[i].LinkList[j].first].Pos;
			ln->update();
			ln->render();
		}
	}
}

void DiEditer::Save(string file)
{
	ofstream fout;
	fout.open(file, ios::out);

	if (fout.is_open())
	{
		fout << Nodes.size() << endl;

		for (int i = 0; i < Nodes.size(); i++)
		{
			fout << Nodes[i].Pos.x << ' ';
			fout << Nodes[i].Pos.y << ' ';
			fout << Nodes[i].Pos.z << ' ';

			fout << Nodes[i].LinkList.size() << ' ';

			for (int j = 0; j < Nodes[i].LinkList.size(); j++)
			{
				fout << Nodes[i].LinkList[j].first << ' ';
				fout << Nodes[i].LinkList[j].second << ' ';
			}
		}

		fout.close();
	}
}

void DiEditer::Load(string file)
{
	ifstream fin;
	fin.open(file, ios::in);

	if (fin.is_open())
	{
		for (int i = 0; i < Nodes.size(); i++)
		{
			Nodes[i].LinkList.clear();
		}
		Nodes.clear();

		int Size;
		fin >> Size;
		Nodes.resize(Size);

		for (int i = 0; i < Nodes.size(); i++)
		{
			fin >> Nodes[i].Pos.x;
			fin >> Nodes[i].Pos.y;
			fin >> Nodes[i].Pos.z;

			fin >> Size;
			Nodes[i].LinkList.resize(Size);

			for (int j = 0; j < Nodes[i].LinkList.size(); j++)
			{
				fin >> Nodes[i].LinkList[j].first;
				fin >> Nodes[i].LinkList[j].second;
			}
		}

		fin.close();
	}
}