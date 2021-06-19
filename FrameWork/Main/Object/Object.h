#pragma once

class Object
{
protected:
	ID3D11Buffer* WorldBuffer;
	ID3D11Buffer* ColorBuffer;
	Matrix S, R, T, W, RT;
	Matrix* P;
	Vector3 Pos;
	Vector3 Scale;
	Vector3 Rot;
	Vector3 Up, Right, Forward;
	Color color;
	Material* Mtl;
	RenderState* renderState;
public:
	Object();
	virtual ~Object();
	virtual HRESULT create();
	virtual void destroy();
	virtual void update();
	virtual void render();

	MAKEREFGET(Matrix, S)
	MAKEREFGET(Matrix, R)
	MAKEREFGET(Matrix, T)
	MAKEREFGET(Matrix, RT)
	MAKEREFGET(Matrix, W)
	MAKEREFGET(Matrix*, P)

	MAKEREFGET(Vector3, Pos)
	MAKEREFGET(Vector3, Scale)
	MAKEREFGET(Vector3, Rot)

	MAKEREFGET(Vector3, Up)
	MAKEREFGET(Vector3, Right)
	MAKEREFGET(Vector3, Forward)

	MAKEREFGET(Color, color)
	MAKEREFGET(Material*, Mtl)
	MAKEREFGET(RenderState*, renderState)

	Vector3 GetWorldPos()
	{
		return Vector3(W._41, W._42, W._43);
	}
};