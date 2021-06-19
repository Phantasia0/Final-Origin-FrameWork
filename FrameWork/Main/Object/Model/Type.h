#pragma once
#define MAX_BONE 256

struct ModelTex
{
	Texture* DiffuseTexture;
	Texture* SpecularTexture;
	Texture* NormalTexture;
	Texture* EmissiveTexture;
	ModelTex() :
		DiffuseTexture(nullptr),
		SpecularTexture(nullptr), NormalTexture(nullptr), EmissiveTexture(nullptr)
	{

	}
};

struct TexMtl
{
	ModelTex tx;
	Mtl mt;
};

struct Mesh
{
	vector<VERTEX_MODEL>Vertices;
	vector<UINT>Indices;

	ID3D11Buffer* vertexBuffer;
	VERTEX_MODEL* vertex;
	UINT vertexCount;

	ID3D11Buffer* IndexBuffer;
	UINT* Index;
	UINT IndexCount;

	string MaterialKey;
};

struct Node
{
	int Index;
	int Parent;
	Matrix Transform;
	string Name = "";
};

struct Bone
{
	string Name = "";
	Matrix Offset;
};

struct AnimPosition
{
	float time;
	Vector3 pos;
};

struct AnimScale
{
	float time;
	Vector3 scale;
};

struct AnimRotation
{
	float time;
	Quaternion quater;
};

struct AnimNode
{
	string name;
	vector<AnimPosition> position;
	vector<AnimScale> scale;
	vector<AnimRotation> rotation;
};

struct Animation
{
	string name;
	float Duration;
	float TickPerSecond;
	map<string, AnimNode*> mapAnimNode;
};

struct BoneAnimation
{
	string name;
	float Duration;
	float TickPerSecond;
	Matrix** ArrBoneTrans;
};

struct BoneArray
{
	Matrix CurrentBones[MAX_BONE];
	Matrix NextBones[MAX_BONE];
	float Bonesweight;
	float HasAnimation;
	float isChanging;
	float ChangingWeight;
	BoneArray() : HasAnimation(false), isChanging(false)
	{

	}
};

#define MAX_WEIGHTS 4
struct VertexWeights
{
	UINT bone_ids[MAX_WEIGHTS];
	float weights[MAX_WEIGHTS];

	void AddData(UINT bone_id, float weight);
	void Normalize();

	VertexWeights()
	{
		ZeroMemory(bone_ids, sizeof(UINT)* MAX_WEIGHTS);
		ZeroMemory(weights, sizeof(float) * MAX_WEIGHTS);
	}
};

static Matrix ToMatrix(const aiMatrix4x4& value)
{
	return Matrix
	(
		value.a1, value.b1, value.c1, value.d1,
		value.a2, value.b2, value.c2, value.d2,
		value.a3, value.b3, value.c3, value.d3,
		value.a4, value.b4, value.c4, value.d4
	);
}

namespace Interpolated
{
	typedef map<string, AnimNode*>::iterator AnimIter;

	Vector3 CalcInterpolatedScaling(AnimIter iter, float time, int Duration);
	Quaternion CalcInterpolatedRotation(AnimIter iter, float time, int Duration);
	Vector3 CalcInterpolatedPosition(AnimIter iter, float time, int Duration);
	int FindScale(AnimIter iter, float time);
	int FindRot(AnimIter iter, float time);
	int FindPos(AnimIter iter, float time);
}