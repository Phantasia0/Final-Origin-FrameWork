#include "stdafx.h"
#include "Astar.h"

void AsTile::ClacH(Int2 End)
{
	H = (abs(Idx.x - End.x) + abs(Idx.y - End.y)) * 10;
}

void AsTile::ClacF()
{
	F = G + H;
}

void AsTile::Reset()
{
	isFind = false;
	P = nullptr;
	F = G = H = INT_MAX;
}

void Astar(vector<vector<AsTile>>& Tiles, vector<AsTile*>& Way, Int2 Start, Int2 End)
{
	priority_queue<PTile, vector<PTile>, compare> Que;

	for (int i = 0; i < Tiles.size(); i++)
	{
		for (int j = 0; j < Tiles[i].size(); j++)
		{
			Tiles[i][j].Reset();
		}
	}

	AsTile* pTemp = &Tiles[Start.x][Start.y];
	pTemp->G = 0;
	pTemp->ClacH(End);
	pTemp->ClacF();
	Que.push({pTemp, pTemp->F});

	while (!Que.empty())
	{
		PTile Temp = Que.top();
		Temp.first->isFind = true;
		Que.pop();

		if (Temp.first->Idx == End)
		{
			break;
		}

		vector<Int2> LoopIdx;
		if (Temp.first->Idx.x > 0)
		{
			LoopIdx.push_back(Int2(Temp.first->Idx.x - 1, Temp.first->Idx.y));
		}
		if (Temp.first->Idx.y > 0)
		{
			LoopIdx.push_back(Int2(Temp.first->Idx.x, Temp.first->Idx.y - 1));
		}
		if (Temp.first->Idx.x < Tiles.size() - 1)
		{
			LoopIdx.push_back(Int2(Temp.first->Idx.x + 1, Temp.first->Idx.y));
		}
		if (Temp.first->Idx.y < Tiles[0].size() - 1)
		{
			LoopIdx.push_back(Int2(Temp.first->Idx.x, Temp.first->Idx.y + 1));
		}

		for (int i = 0; i < LoopIdx.size(); i++)
		{
			AsTile* loop = &Tiles[LoopIdx[i].x][LoopIdx[i].y];

			if (loop->State != TILE_WALL)
			{
				loop->ClacH(End);
				if (loop->G > Temp.first->G + 10)
				{
					loop->G = Temp.first->G + 10;
					loop->ClacF();
					loop->P = Temp.first;

					if (!loop->isFind)
					{
						Que.push({ loop,loop->F });
					}
				}
			}
		}

		LoopIdx.clear();
	}

	AsTile* p = &Tiles[End.x][End.y];
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