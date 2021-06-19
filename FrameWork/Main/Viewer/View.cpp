#include "stdafx.h"
#include "View.h"

View::View(Vector3 eye, Vector3 look, Vector3 up)
{
	D3DXMatrixIdentity(&matrix);
	this->eye = eye;
	this->look = look;
	this->up = up;
}

View::~View()
{
}

void View::GetMatrix(Matrix * matrix)
{
	memcpy(matrix, &this->matrix, sizeof(Matrix));
}

void View::Set(Vector3 eye, Vector3 look, Vector3 up)
{
	D3DXMatrixLookAtLH(&matrix, &eye, &look, &up);
}

void View::Set(Matrix * mat)
{
	matrix = *mat;
}