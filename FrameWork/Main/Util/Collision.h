#pragma once
#include "stdafx.h"

template<typename T>
static T Saturate(T& value)
{
	if (value > 1) value = 1;
	if (value < 0) value = 0;
	return value;
}

static float GetAxisYSeta(Vector3& sour, Vector3& dest)
{
	Vector3 Dir = dest - sour;
	D3DXVec3Normalize(&Dir, &Dir);

	if (Dir.z < 0.0f)
	{
		return acosf(Dir.x);
	}
	else
	{
		return 2.0f * PI - acosf(Dir.x);
	}
}

static void LinearRot(float& sourRot, float& destRot)
{
	if (sourRot > 2.0f * PI)
	{
		sourRot -= 2.0f * PI;
	}
	if (sourRot < 0.0f)
	{
		sourRot = 2.0f*PI + sourRot;
	}
	if (destRot - sourRot > PI)
	{
		destRot = (2.0f*PI - destRot)*-1.0f;
	}
	else if (destRot - sourRot < -PI)
	{
		destRot += 2.0f * PI;
	}
}

static Vector2 ClacUiWorld(POINT pt)
{
	Vector2 temp;
	temp.x = (float)pt.x - g_D3dDesc.Width *0.5f;
	temp.y = g_D3dDesc.Height*0.5f - (float)pt.y;
	return temp;
}

static bool PtInRect(Ui* ui, POINT pt)
{
	COL_CUBE rect;

	D3DXVec3TransformCoord(&rect.min, &rect.min, &ui->GetRefW());
	D3DXVec3TransformCoord(&rect.max, &rect.max, &ui->GetRefW());

	Vector2 temp = ClacUiWorld(pt);

	if (rect.min.x <  temp.x && temp.x < rect.max.x&&
		rect.min.y <  temp.y && temp.y < rect.max.y)
	{
		return true;
	}
	return false;
}

static float GetDistance(Vector3& sour, Vector3& dest)
{
	Vector3 temp = dest - sour;
	return D3DXVec3Length(&temp);
}

static bool IntersectSphere(COL_SPHERE& a, COL_SPHERE& b)
{
	float Dis = GetDistance(a.center, b.center);
	return Dis <= (a.radius + b.radius);
}

static bool IntersectSphere(Sphere*& a, Sphere*& b)
{
	return IntersectSphere(a->GetRefcol(), b->GetRefcol());
}

static bool AABB(Cube* cb1, Cube* cb2)
{
	if (cb1->GetRefcol().min.x < cb2->GetRefcol().max.x &&
		cb1->GetRefcol().max.x > cb2->GetRefcol().min.x  &&
		cb1->GetRefcol().min.y < cb2->GetRefcol().max.y &&
		cb1->GetRefcol().max.y > cb2->GetRefcol().min.y &&
		cb1->GetRefcol().min.z < cb2->GetRefcol().max.z &&
		cb1->GetRefcol().max.z > cb2->GetRefcol().min.z)
	{
		return true;
	}
	return false;
}

static bool OBB(Cube* cb1, Cube* cb2)
{
	Vector3 cb1half = cb1->GetRefScale()*0.5f;
	Vector3 cb2half = cb2->GetRefScale()*0.5f;
	Vector3 dis = cb1->GetWorldPos() - cb2->GetWorldPos();

	float c[3][3];
	float ac[3][3];
	float proj[3];
	float d, e0, e1;

	c[0][0] = D3DXVec3Dot(&cb1->GetRefRight(), &cb2->GetRefRight());
	c[0][1] = D3DXVec3Dot(&cb1->GetRefRight(), &cb2->GetRefUp());
	c[0][2] = D3DXVec3Dot(&cb1->GetRefRight(), &cb2->GetRefForward());

	ac[0][0] = fabs(c[0][0]);
	ac[0][1] = fabs(c[0][1]);
	ac[0][2] = fabs(c[0][2]);

	proj[0] = D3DXVec3Dot(&cb1->GetRefRight(), &dis);
	d = fabs(proj[0]);
	e0 = cb1half.x;
	e1 = cb2half.x * ac[0][0] + cb2half.y * ac[0][1] + cb2half.z * ac[0][2];
	if (d > e0 + e1) return false;

	c[1][0] = D3DXVec3Dot(&cb1->GetRefUp(), &cb2->GetRefRight());
	c[1][1] = D3DXVec3Dot(&cb1->GetRefUp(), &cb2->GetRefUp());
	c[1][2] = D3DXVec3Dot(&cb1->GetRefUp(), &cb2->GetRefForward());

	ac[1][0] = fabs(c[1][0]);
	ac[1][1] = fabs(c[1][1]);
	ac[1][2] = fabs(c[1][2]);

	proj[1] = D3DXVec3Dot(&cb1->GetRefUp(), &dis);
	d = fabs(proj[1]);
	e0 = cb1half.y;
	e1 = cb2half.x * ac[1][0] + cb2half.y * ac[1][1] + cb2half.z * ac[1][2];
	if (d > e0 + e1) return false;

	c[2][0] = D3DXVec3Dot(&cb1->GetRefForward(), &cb2->GetRefRight());
	c[2][1] = D3DXVec3Dot(&cb1->GetRefForward(), &cb2->GetRefUp());
	c[2][2] = D3DXVec3Dot(&cb1->GetRefForward(), &cb2->GetRefForward());

	ac[2][0] = fabs(c[2][0]);
	ac[2][1] = fabs(c[2][1]);
	ac[2][2] = fabs(c[2][2]);

	proj[2] = D3DXVec3Dot(&cb1->GetRefForward(), &dis);
	d = fabs(proj[2]);
	e0 = cb1half.z;
	e1 = cb2half.x * ac[2][0] + cb2half.y * ac[2][1] + cb2half.z * ac[2][2];
	if (d > e0 + e1) return false;

	d = fabs(D3DXVec3Dot(&cb2->GetRefRight(), &dis));
	e0 = cb2half.x;
	e1 = cb1half.x * ac[0][0] + cb1half.y * ac[1][0] + cb1half.z * ac[2][0];
	if (d > e0 + e1) return false;

	d = fabs(D3DXVec3Dot(&cb2->GetRefUp(), &dis));
	e0 = cb2half.y;
	e1 = cb1half.x * ac[0][1] + cb1half.y * ac[1][1] + cb1half.z * ac[2][1];
	if (d > e0 + e1) return false;

	d = fabs(D3DXVec3Dot(&cb2->GetRefForward(), &dis));
	e0 = cb2half.z;
	e1 = cb1half.x * ac[0][2] + cb1half.y * ac[1][2] + cb1half.z * ac[2][2];
	if (d > e0 + e1) return false;

	d = fabs(proj[2] * c[1][0] - proj[1] * c[2][0]);
	e0 = cb1half.y * ac[2][0] + cb1half.z * ac[1][0];
	e1 = cb2half.y * ac[0][2] + cb2half.z * ac[0][1];
	if (d > e0 + e1) return false;

	d = fabs(proj[2] * c[1][1] - proj[1] * c[2][1]);
	e0 = cb1half.y * ac[2][1] + cb1half.z * ac[1][1];
	e1 = cb2half.x * ac[0][2] + cb2half.z * ac[0][0];
	if (d > e0 + e1) return false;

	d = fabs(proj[2] * c[1][2] - proj[1] * c[2][2]);
	e0 = cb1half.y * ac[2][2] + cb1half.z * ac[1][2];
	e1 = cb2half.x * ac[0][1] + cb2half.y * ac[0][0];
	if (d > e0 + e1) return false;

	d = fabs(proj[0] * c[2][0] - proj[2] * c[0][0]);
	e0 = cb1half.x * ac[2][0] + cb1half.z * ac[0][0];
	e1 = cb2half.y * ac[1][2] + cb2half.z * ac[1][1];
	if (d > e0 + e1) return false;

	d = fabs(proj[0] * c[2][1] - proj[2] * c[0][1]);
	e0 = cb1half.x * ac[2][1] + cb1half.z * ac[0][1];
	e1 = cb2half.x * ac[1][2] + cb2half.z * ac[1][0];
	if (d > e0 + e1) return false;

	d = fabs(proj[0] * c[2][2] - proj[2] * c[0][2]);
	e0 = cb1half.x * ac[2][2] + cb1half.z * ac[0][2];
	e1 = cb2half.x * ac[1][1] + cb2half.y * ac[1][0];
	if (d > e0 + e1) return false;

	d = fabs(proj[1] * c[0][0] - proj[0] * c[1][0]);
	e0 = cb1half.x * ac[1][0] + cb1half.y * ac[0][0];
	e1 = cb2half.y * ac[2][2] + cb2half.z * ac[2][1];
	if (d > e0 + e1) return false;

	d = fabs(proj[1] * c[0][1] - proj[0] * c[1][1]);
	e0 = cb1half.x * ac[1][1] + cb1half.y * ac[0][1];
	e1 = cb2half.x * ac[2][2] + cb2half.z * ac[2][0];
	if (d > e0 + e1) return false;

	d = fabs(proj[1] * c[0][2] - proj[0] * c[1][2]);
	e0 = cb1half.x * ac[1][2] + cb1half.y * ac[0][2];
	e1 = cb2half.x * ac[2][1] + cb2half.y * ac[2][0];
	if (d > e0 + e1) return false;

	return true;
}

static bool AABBSphere(COL_CUBE& box, COL_SPHERE& sp)
{
	Vector3 collision_box_near_point;

	if (sp.center.x < box.min.x)
	{
		collision_box_near_point.x = box.min.x;
	}
	else if (sp.center.x > box.max.x)
	{
		collision_box_near_point.x = box.max.x;
	}
	else
	{
		collision_box_near_point.x = sp.center.x;
	}

	if (sp.center.y < box.min.y)
	{
		collision_box_near_point.y = box.min.y;
	}
	else if (sp.center.y > box.max.y)
	{
		collision_box_near_point.y = box.max.y;
	}
	else
	{
		collision_box_near_point.y = sp.center.y;
	}

	if (sp.center.z < box.min.z)
	{
		collision_box_near_point.z = box.min.z;
	}
	else if (sp.center.z > box.max.z)
	{
		collision_box_near_point.z = box.max.z;
	}
	else
	{
		collision_box_near_point.z = sp.center.z;
	}

	float Collision_check_distance = GetDistance(sp.center, collision_box_near_point);

	if (Collision_check_distance < sp.radius)
	{
		return true;
	}

	return false;
}

static bool AABBSphere(Cube* cb, Sphere* sp)
{
	COL_CUBE col1 = cb->GetRefcol();
	COL_SPHERE col2 = sp->GetRefcol();

	return AABBSphere(col1, col2);
}

static bool OBBSphere(Cube* cb, Sphere* sp)
{
	Matrix Inverse;
	D3DXMatrixInverse(&Inverse, NULL, &cb->GetRefRT());
	COL_SPHERE col = sp->GetRefcol();

	D3DXVec3TransformCoord(&col.center, &col.center, &Inverse);
	COL_CUBE col1 = cb->GetRefcol();

	D3DXVec3TransformCoord(&col1.min, &col1.min, &Inverse);
	D3DXVec3TransformCoord(&col1.max, &col1.max, &Inverse);

	return AABBSphere(col1, col);
}

static bool RayCastCube(Cube* cb, Ray Wray, OUT Vector3& target, float Dis)
{
	Matrix Inverse;
	D3DXMatrixInverse(&Inverse, NULL, &cb->GetRefW());

	D3DXVec3TransformCoord(&Wray.Pos, &Wray.Pos, &Inverse);
	D3DXVec3TransformNormal(&Wray.Dir, &Wray.Dir, &Inverse);
	D3DXVec3Normalize(&Wray.Dir, &Wray.Dir);

	float u, v, dis;
	for (int i = 0; i < 36; i += 3)
	{
		if (D3DXIntersectTri(&cb->GetRefvertex()[i].Position,
			&cb->GetRefvertex()[i + 1].Position,
			&cb->GetRefvertex()[i + 2].Position,
			&Wray.Pos, &Wray.Dir, &u, &v, &dis))
		{
			if (dis > Dis) return false;
			target = Wray.Pos + dis * Wray.Dir;
			D3DXVec3TransformCoord(&target, &target, &cb->GetRefW());

			return true;
		}
	}

	return false;
}

static bool RayCastSphere(Sphere* sp, Ray Wray)
{
	Wray.Pos -= sp->GetWorldPos();

	float b = D3DXVec3Dot(&Wray.Pos, &Wray.Dir);
	float c = D3DXVec3Dot(&Wray.Pos, &Wray.Pos)
		- sp->GetRefcol().radius * sp->GetRefcol().radius;

	return b * b - c >= 0;
}

static Ray ClacPickingRay(POINT pt, Cam* cam)
{
	Ray temp;
	float px, py;
	Matrix V, P, Inverse;

	pt.x -= (LONG)cam->GetRefVP()->GetX();
	pt.y -= (LONG)cam->GetRefVP()->GetY();

	cam->GetRefProj()->GetMatrix(&P);
	px = ((2.0f*pt.x) / cam->GetRefVP()->GetWidth() - 1.0f) / P._11;
	py = ((-2.0f*pt.y) / cam->GetRefVP()->GetHeight() + 1.0f) / P._22;

	cam->GetRefV()->GetMatrix(&V);
	D3DXMatrixInverse(&Inverse, 0, &V);

	temp.Pos = cam->GetRefWorldEye();
	temp.Dir = Vector3(px, py, 1.0f);

	D3DXVec3TransformNormal(&temp.Dir, &temp.Dir, &Inverse);
	D3DXVec3Normalize(&temp.Dir, &temp.Dir);

	return temp;
}

static bool PlaneinRay(IN Ray Wray, IN Plane* pl, OUT Vector3& target)
{
	Matrix inverse;

	D3DXMatrixInverse(&inverse, 0, &pl->GetRefW());

	D3DXVec3TransformCoord(&Wray.Pos, &Wray.Pos, &inverse);
	D3DXVec3TransformNormal(&Wray.Dir, &Wray.Dir, &inverse);
	D3DXVec3Normalize(&Wray.Dir, &Wray.Dir);

	float u, v, dis;

	for (int i = 0; i < 6; i += 3)
	{
		if (D3DXIntersectTri(&pl->GetRefvertex()[i].Position,
			&pl->GetRefvertex()[i + 1].Position,
			&pl->GetRefvertex()[i + 2].Position,
			&Wray.Pos, &Wray.Dir, &u, &v, &dis))
		{
			target = Wray.Pos + dis * Wray.Dir;
			D3DXVec3TransformCoord(&target, &target, &pl->GetRefW());

			return true;
		}
	}

	return false;
}

static bool HeightinRay(IN Ray Wray, IN HeightMap* Hm, OUT Vector3& target)
{
	Matrix inverse;
	Vector2 IndPos;

	D3DXMatrixInverse(&inverse, 0, &Hm->GetRefW());

	D3DXVec3TransformNormal(&Wray.Dir, &Wray.Dir, &inverse);
	D3DXVec3Normalize(&Wray.Dir, &Wray.Dir);
	D3DXVec3TransformCoord(&Wray.Pos, &Wray.Pos, &inverse);

	float MaxScale = Hm->GetRefMaxX() * Hm->GetRefScale();

	IndPos.x = Wray.Pos.x + MaxScale * 0.5f;
	IndPos.y = Wray.Pos.z - MaxScale * 0.5f;

	int indx, indz;

	indx = int(IndPos.x / Hm->GetRefScale());
	indz = int(IndPos.y / Hm->GetRefScale() * -1.0f);

	if (indx >= Hm->GetRefMaxX() - 1 || indx < 0
		|| indz >= Hm->GetRefMaxX() - 1 || indz < 0)
	{
		return false;
	}

	float u, v, dis;
	int index = int((6.0f * (Hm->GetRefMaxX() - 1))*indz + (6.0f * indx));

	for (int i = 0; i<6; i += 3)
	{
		if (D3DXIntersectTri(&Hm->GetRefvertex()[index + i].Position,
			&Hm->GetRefvertex()[index + i + 1].Position,
			&Hm->GetRefvertex()[index + i + 2].Position,
			&Wray.Pos, &Wray.Dir, &u, &v, &dis))
		{
			target = Wray.Pos + dis * Wray.Dir;
			D3DXVec3TransformCoord(&target, &target, &Hm->GetRefW());

			return true;
		}
	}

	return false;
}

static bool HeightinRay2(IN Ray Wray, IN HeightMap* Hm, OUT Vector3& target)
{
	Matrix inverse;

	D3DXMatrixInverse(&inverse, 0, &Hm->GetRefW());

	D3DXVec3TransformNormal(&Wray.Dir, &Wray.Dir, &inverse);
	D3DXVec3Normalize(&Wray.Dir, &Wray.Dir);
	D3DXVec3TransformCoord(&Wray.Pos, &Wray.Pos, &inverse);

	float u, v, dis;
	float min = FLT_MAX;
	Vector3 minTarget;

	for (int i = 0; i < Hm->GetRefvertexCount(); i += 3)
	{
		if (D3DXIntersectTri(&Hm->GetRefvertex()[i].Position,
			&Hm->GetRefvertex()[i + 1].Position,
			&Hm->GetRefvertex()[i + 2].Position,
			&Wray.Pos, &Wray.Dir, &u, &v, &dis))
		{
			if (dis < min)
			{
				min = dis;
				minTarget = Wray.Pos + dis * Wray.Dir;
			}
		}
	}

	if (min != FLT_MIN)
	{
		target = minTarget;
		D3DXVec3TransformCoord(&target, &target, &Hm->GetRefW());

		return true;
	}

	return false;
}


static int RndInt(int to, int from)
{
	return rand() % (from - to + 1) + to;
}

static float RndFloat(float to = 0.0f, float from = 1.0f)
{
	float temp = (float)rand() / (float)RAND_MAX * (from - to) + to;
	return temp;
}