#pragma once

struct Int2
{
	int x, y;
	Int2(){}
	Int2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	bool operator==(Int2& b)
	{
		return (x == b.x && y == b.y);
	}
};

enum TILESTATE
{
	TILE_NONE,
	TILE_WALL,
	TILE_START,
	TILE_END
};

class AsTile
{
public:
	int F, G, H;
	int State;
	Int2 Idx;
	AsTile* P;
	bool isFind;
	void ClacH(Int2 End);
	void ClacF();
	void Reset();
};

using PTile = pair<AsTile*, int>;

struct compare
{
	bool operator()(PTile& a, PTile& b)
	{
		return a.second > b.second;
	}
};

void Astar(vector<vector<AsTile>>& Tiles, _Inout_ vector<AsTile*>& Way, Int2 Start, Int2 End);