#pragma once
#include "Type.h"
using namespace Interpolated;

enum ANI_STATE
{
	ANI_LOOPPLAY,
	ANI_ONCEPLAY,
	ANI_STOP
};

struct AnimState
{
	bool isPlaying;
	int Index;
	int CurrentFrame;
	int NextFrame;
	float Weight;
	int State;
	AnimState()
	{
		isPlaying = false;
		Index = 0;
		CurrentFrame = 0;
		NextFrame = 1;
		Weight = 0.0f;
		State = ANI_STOP;
	}
};

class Model : public Object
{
private:
	_tstring file;
	_tstring path;
	UINT boneCount = 0;
	AnimState PlayAnimState;
	AnimState ChangeAnimState;
	float AnimBlendTime;
	float AnimCurrentBlendTime = 0.0f;
	bool isCopy = false;
private:
	const aiScene* scene;
private:
	Shader* shader;
	Texture* texture;
	map<string, TexMtl> TtMap;

	ID3D11Buffer* BoneArrayBuffer;
	BoneArray BoneData;

	ID3D11Buffer* ChangeBoneArrayBuffer;
	BoneArray ChangeBoneData;

	vector<Mesh*> vecMeshes;
	map<string, TexMtl> mapMaterial;
	vector<BoneAnimation*> BoneAnimations;
	vector<Bone*> vecBones;

	map<string, UINT> mapBone;
	map<string, Matrix> mapNode;
	vector<Node*> vecNodes;
	vector<Animation*> vecAnimations;
private:
	void ReadNode(aiNode* node, int index, int parent);
	void ReadMesh(aiNode* node);
	void ReadBoneData(aiMesh* mesh, vector<VertexWeights>& vertex_weights);
	void ReadMaterial();
	void ReadAnimation();
	void ReadBone();
	void MakeBoneAnimation();
	void BlendAnimation();
	void PlayAnimation(AnimState& AnimState);
public:
	Model(_tstring key = _T("Shader/8.Model.hlsl"), string input = "MODEL");
	Model(Model* Copy);
	~Model() override;
	bool ReadFile(_tstring file, _tstring path = _T(""));
	bool ReadAnimation(_tstring file, _tstring path = _T(""));
	void destroy() override;
	void update() override;
	void render() override;
	void releaseNode();
	void ChangeAnimation(int Index, int State, float BlendTime);
	void SetVertex(Matrix srt);
	Matrix GetBone(string str);
	MAKEREFGET(AnimState, PlayAnimState)
	MAKEREFGET(BoneArray, BoneData)
	MAKEREFGET(Shader*, shader)
};