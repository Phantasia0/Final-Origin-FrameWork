#include "stdafx.h"
#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::Update()
{
	Matrix V, P, VP;
	pCam->GetRefProj()->GetMatrix(&P);
	pCam->GetRefV()->GetMatrix(&V);

	VP = V * P;

	a = VP._14 + VP._11;
	b = VP._24 + VP._21;
	c = VP._34 + VP._31;
	d = VP._44 + VP._41;
	planes[0] = { a, b, c, d };

	a = VP._14 - VP._11;
	b = VP._24 - VP._21;
	c = VP._34 - VP._31;
	d = VP._44 - VP._41;
	planes[1] = { a, b, c, d };

	a = VP._14 + VP._12;
	b = VP._24 + VP._22;
	c = VP._34 + VP._32;
	d = VP._44 + VP._42;
	planes[2] = { a, b, c, d };

	a = VP._14 - VP._12;
	b = VP._24 - VP._22;
	c = VP._34 - VP._32;
	d = VP._44 - VP._42;
	planes[3] = { a, b, c, d };

	a = VP._14 + VP._13;
	b = VP._24 + VP._23;
	c = VP._34 + VP._33;
	d = VP._44 + VP._43;
	planes[4] = { a, b, c, d };

	a = VP._14 - VP._13;
	b = VP._24 - VP._23;
	c = VP._34 - VP._33;
	d = VP._44 - VP._43;
	planes[5] = { a, b, c, d };

	for (UINT i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&planes[i], &planes[i]);
	}
}

bool Frustum::ContainPoint(Vector3 point)
{
	for (UINT i = 0; i < 6; i++)
	{
		float dot = D3DXPlaneDotCoord(&planes[i], &point);

		if (dot < 0.0f) return false;
	}
	return true;
}

bool Frustum::ContainSphere(Vector3 center, float radius)
{
	Vector3 edge;
	float dot;

	for (UINT i = 0; i < 6; i++)
	{
		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;

		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;
		dot = D3DXPlaneDotCoord(&planes[i], &edge);
		if (dot > 0.0f)
			continue;

		return false;
	}

	return true;
}