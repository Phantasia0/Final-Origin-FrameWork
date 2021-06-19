#pragma once
#include "stdafx.h"

static Vector3 Linear(Vector3 from, Vector3 to, float g)
{
	return from + (to - from) * g;
}

static float Linear(float from, float to, float g)
{
	return from + (to - from) * g;
}

static Vector3 Quadratic(Vector3 from, Vector3 to, Vector3 via, float g)
{
	Vector3 m0, m1;
	m0 = Linear(from, to, g);
	m1 = Linear(to, via, g);

	return Linear(m0, m1, g);
}

static Vector3 Cubic(Vector3 from, Vector3 to1, Vector3 to2, Vector3 via, float g)
{
	Vector3 m0, m1, m2;
	m0 = Linear(from, to1, g);
	m1 = Linear(to1, to2, g);
	m2 = Linear(to2, via, g);

	return Quadratic(m0, m1, m2, g);
}