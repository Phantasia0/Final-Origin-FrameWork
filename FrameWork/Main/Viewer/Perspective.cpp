#include "stdafx.h"
#include "Perspective.h"

Perspective::Perspective(float width, float height, float zn, float zf, float fov)
	:Projection(width,height,zn,zf,fov)
{
	Set(width, height, zn, zf, fov);
}

Perspective::~Perspective()
{
}

void Perspective::Set(float width, float height, float zn, float zf, float fov)
{
	__super::Set(width, height, zn, zf, fov);

	aspect = width / height;
	D3DXMatrixPerspectiveFovLH(&matrix, fov, aspect, zn, zf);
}

void Perspective::Set(Matrix * mat)
{
	matrix = *mat;
}