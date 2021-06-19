#pragma once
#include "stdafx.h"

struct COL_SPHERE
{
	Vector3 center;
	float radius;
};

struct COL_CUBE
{
	Vector3 min;
	Vector3 max;
	COL_CUBE()
	{
		min = Vector3(-0.5f, -0.5f, -0.5f);
		max = Vector3(0.5f, 0.5f, 0.5f);
	}
};

typedef struct COL_RAY
{
	Vector3 Pos;
	Vector3 Dir;
}Ray;