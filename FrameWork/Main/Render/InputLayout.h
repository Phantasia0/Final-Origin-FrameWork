#pragma once

struct VERTEX_P
{
	VERTEX_P()
		:Position(0,0,0) {}
	Vector3 Position;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_PC
{
	VERTEX_PC()
		:Position(0,0,0), Color(0,0,0){}
	Vector3 Position;
	Vector3 Color;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_PT
{
	VERTEX_PT()
		:Position(0, 0, 0), Uv(0, 0) {}
	Vector3 Position;
	Vector2 Uv;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_PS
{
	VERTEX_PS()
		:Position(0, 0, 0), Size(0, 0) {}
	Vector3 Position;
	Vector2 Size;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_PSV
{
	VERTEX_PSV()
		:Position(0, 0, 0), Size(0, 0), Velocity(0, 0, 0) {}
	Vector3 Position;
	Vector2 Size;
	Vector3 Velocity;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_PCN
{
	VERTEX_PCN()
		:Position(0, 0, 0), Color(0, 0, 0), Normal(0, 0, 0) {}
	Vector3 Position;
	Vector3 Color;
	Vector3 Normal;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_PTN
{
	VERTEX_PTN()
		:Position(0, 0, 0), Uv(0, 0), Normal(0, 0, 0) {}
	Vector3 Position;
	Vector2 Uv;
	Vector3 Normal;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};

struct VERTEX_MODEL
{
	VERTEX_MODEL()
		:Position(0, 0, 0), Uv(0, 0), Normal(0, 0, 0),
		Tangent(0, 0, 0), Indices(0, 0, 0, 0), Weights(0, 0, 0, 0) {}
	Vector3 Position;
	Vector2 Uv;
	Vector3 Normal;
	Vector3 Tangent;
	Vector4 Indices;
	Vector4 Weights;
	static D3D11_INPUT_ELEMENT_DESC LayoutDesc[];
};