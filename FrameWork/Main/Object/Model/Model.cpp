#include "stdafx.h"
#include "Model.h"

Model::Model(_tstring key, string input)
{
	shader = SHADERMANAGER->AddShader(key, input);

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoneArray);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &BoneArrayBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoneArray);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ChangeBoneArrayBuffer);
		assert(SUCCEEDED(hr));
	}
}

Model::Model(Model * Copy)
{
}

Model::~Model()
{
}

bool Model::ReadFile(_tstring file, _tstring path)
{
	__super::create();

	Assimp::Importer importer;

	this->file = file;
	this->path = path;

	scene = importer.ReadFile
	(
		path + file,
		aiProcess_ConvertToLeftHanded
		| aiProcess_Triangulate
		| aiProcess_GenUVCoords
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace
	);
	assert(scene != NULL);

	ReadMaterial();
	ReadNode(scene->mRootNode, -1, -1);
	ReadMesh(scene->mRootNode);

	if (scene->HasAnimations())
	{
		BoneData.HasAnimation = 1.0f;
		ReadAnimation();
		MakeBoneAnimation();
	}

	importer.FreeScene();

	return true;
}

bool Model::ReadAnimation(_tstring file, _tstring path)
{
	__super::create();

	Assimp::Importer importer;

	this->file = file;
	this->path = path;

	scene = importer.ReadFile
	(
		path + file,
		aiProcess_ConvertToLeftHanded
		| aiProcess_Triangulate
		| aiProcess_GenUVCoords
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace
	);
	assert(scene != NULL);

	if (scene->HasAnimations())
	{
		BoneData.HasAnimation = 1.0f;
		ReadAnimation();
		MakeBoneAnimation();
	}

	importer.FreeScene();
	return false;
}

void Model::ReadNode(aiNode * node, int index, int parent)
{
	Node* _node = new Node();
	_node->Index = index;
	_node->Parent = parent;
	_node->Name = node->mName.C_Str();
	Matrix transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&_node->Transform, &transform);

	Matrix pM;

	if (parent < 0)
	{
		D3DXMatrixIdentity(&pM);
	}
	else
	{
		pM = vecNodes[parent]->Transform;
	}

	_node->Transform *= pM;

	vecNodes.emplace_back(_node);
	mapNode[_node->Name] = _node->Transform;

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ReadNode(node->mChildren[i], vecNodes.size(), index);
	}
}

void Model::ReadMesh(aiNode * node)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		Mesh* mesh = new Mesh();
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		mesh->MaterialKey = scene->mMaterials[srcMesh->mMaterialIndex]->GetName().C_Str();
		mesh->vertexCount = srcMesh->mNumVertices;
		
		vector<VertexWeights> vertex_weights;
		mesh->Vertices.resize(mesh->vertexCount);
		vertex_weights.resize(mesh->vertexCount);

		ReadBoneData(srcMesh, vertex_weights);

		for (UINT j = 0; j < mesh->vertexCount; j++)
		{
			memcpy(&mesh->Vertices[j].Position, &srcMesh->mVertices[j], sizeof(Vector3));

			if (srcMesh->HasTextureCoords(0))
			{
				memcpy(&mesh->Vertices[j].Uv, &srcMesh->mTextureCoords[0][j], sizeof(Vector2));
			}
			if (srcMesh->HasNormals())
			{
				memcpy(&mesh->Vertices[j].Normal, &srcMesh->mNormals[j], sizeof(Vector3));
			}
			if (srcMesh->HasTangentsAndBitangents())
			{
				memcpy(&mesh->Vertices[j].Tangent, &srcMesh->mTangents[j], sizeof(Vector3));
			}

			if (!vertex_weights.empty())
			{
				vertex_weights[j].Normalize();

				mesh->Vertices[j].Indices.x = static_cast<float>(vertex_weights[j].bone_ids[0]);
				mesh->Vertices[j].Indices.y = static_cast<float>(vertex_weights[j].bone_ids[1]);
				mesh->Vertices[j].Indices.z = static_cast<float>(vertex_weights[j].bone_ids[2]);
				mesh->Vertices[j].Indices.w = static_cast<float>(vertex_weights[j].bone_ids[3]);

				mesh->Vertices[j].Weights.x = vertex_weights[i].weights[0];
				mesh->Vertices[j].Weights.y = vertex_weights[j].weights[1];
				mesh->Vertices[j].Weights.z = vertex_weights[j].weights[2];
				mesh->Vertices[j].Weights.w = vertex_weights[j].weights[3];
			}
		}

		UINT IndexCount = srcMesh->mNumFaces;
		for (UINT j = 0; j < IndexCount; j++)
		{
			aiFace& face = srcMesh->mFaces[j];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->Indices.emplace_back(face.mIndices[k]);
			}
		}

		mesh->vertex = new VERTEX_MODEL[mesh->vertexCount];
		copy(mesh->Vertices.begin(), mesh->Vertices.end(), stdext::checked_array_iterator<VERTEX_MODEL*>
			(mesh->vertex, mesh->vertexCount));

		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.ByteWidth = sizeof(VERTEX_MODEL) & mesh->vertexCount;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = mesh->vertex;

			CHECK(g_Device->CreateBuffer(&desc, &subResource, &mesh->vertexBuffer));
		}

		mesh->IndexCount = mesh->Indices.size();
		mesh->Index = new UINT[mesh->IndexCount];
		copy(mesh->Indices.begin(), mesh->Indices.end(), stdext::checked_array_iterator<UINT*>
			(mesh->Index, mesh->IndexCount));

		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.ByteWidth = sizeof(UINT) * mesh->IndexCount;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = mesh->Index;
			CHECK(g_Device->CreateBuffer(&desc, &subResource, &mesh->IndexBuffer));
		}

		mesh->Vertices.clear();
		mesh->Vertices.shrink_to_fit();
		mesh->Indices.clear();
		mesh->Indices.shrink_to_fit();

		vecMeshes.emplace_back(mesh);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ReadMesh(node->mChildren[i]);
	}
}

void Model::ReadBoneData(aiMesh * mesh, vector<VertexWeights>& vertex_weights)
{
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		UINT boneIndex = 0;
		string name = mesh->mBones[i]->mName.C_Str();

		if (mapBone.find(name) == mapBone.end())
		{
			boneIndex = boneCount;
			boneCount++;

			mapBone[name] = boneIndex;

			Bone* srcBone = new Bone();
			srcBone->Name = name;
			srcBone->Offset = ToMatrix(mesh->mBones[i]->mOffsetMatrix);

			vecBones.emplace_back(srcBone);
		}
		else
		{
			boneIndex = mapBone[name];
		}

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			UINT vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
			vertex_weights[vertex_id].AddData(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void Model::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMtl = scene->mMaterials[i];
		aiColor3D color;
		float Shininess;
		float opacity;
		string MtlKey;
		TexMtl texmtl = {};

		MtlKey = srcMtl->GetName().C_Str();

		srcMtl->Get(AI_MATKEY_COLOR_AMBIENT, color);
		texmtl.mt.Ambient.x = color.r;
		texmtl.mt.Ambient.y = color.g;
		texmtl.mt.Ambient.z = color.b;

		srcMtl->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		texmtl.mt.Diffuse.x = color.r;
		texmtl.mt.Diffuse.y = color.g;
		texmtl.mt.Diffuse.z = color.b;

		srcMtl->Get(AI_MATKEY_COLOR_SPECULAR, color);
		texmtl.mt.Specular.x = color.r;
		texmtl.mt.Specular.y = color.g;
		texmtl.mt.Specular.z = color.b;

		srcMtl->Get(AI_MATKEY_SHININESS, Shininess);
		texmtl.mt.Specular.w = Shininess;

		srcMtl->Get(AI_MATKEY_OPACITY, opacity);

		map<string, TexMtl>::iterator Texiter;
		Texiter = TtMap.find(MtlKey);

		if (Texiter != TtMap.end())
		{
			texmtl.mt.HasMap = Texiter->second.mt.HasMap;
			texmtl.tx.DiffuseTexture = Texiter->second.tx.DiffuseTexture;
			texmtl.tx.EmissiveTexture = Texiter->second.tx.EmissiveTexture;
			texmtl.tx.NormalTexture = Texiter->second.tx.NormalTexture;
			texmtl.tx.SpecularTexture = Texiter->second.tx.SpecularTexture;
		}
		else
		{
			{
				aiString file;
				aiReturn texFound;
				string str;

				texFound = srcMtl->GetTexture(aiTextureType_DIFFUSE, 0, &file);
				str = file.C_Str();
				size_t index = str.find_last_of('/');
				str = str.substr(index + 1, str.length());
				string Texkey = str;

				if (texFound == AI_SUCCESS && str != "")
				{
					texmtl.tx.DiffuseTexture = new Texture();
					texmtl.tx.DiffuseTexture->CreateSampler();
					texmtl.tx.DiffuseTexture->CreateTexture(str, path);
					texmtl.mt.HasMap.x = 1.0f;
				}
			}

			{
				aiString file;
				aiReturn texFound;
				string str;

				texFound = srcMtl->GetTexture(aiTextureType_NORMALS, 0, &file);
				str = file.C_Str();
				size_t index = str.find_last_of('/');
				str = str.substr(index + 1, str.length());
				string Texkey = str;

				if (texFound == AI_SUCCESS && str != "")
				{
					texmtl.tx.NormalTexture = new Texture();
					texmtl.tx.NormalTexture->CreateSampler();
					texmtl.tx.NormalTexture->CreateTexture(str, path);
					texmtl.mt.HasMap.y = 1.0f;
				}
			}
		}

		mapMaterial.insert(make_pair(MtlKey, texmtl));
	}
}

void Model::ReadAnimation()
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		Animation* Anim = new Animation();
		aiAnimation* srcAnim = scene->mAnimations[i];

		Anim->name = srcAnim->mName.C_Str();
		Anim->Duration = (float)srcAnim->mDuration;
		Anim->TickPerSecond = srcAnim->mTicksPerSecond != 0.0 ? (float)srcAnim->mTicksPerSecond : 25.0f;

		for (UINT j = 0; j < srcAnim->mNumChannels; j++)
		{
			AnimNode* animNode = new AnimNode();
			aiNodeAnim* srcAnimNode = srcAnim->mChannels[i];

			animNode->name = srcAnimNode->mNodeName.C_Str();

			for (UINT k = 0; k < srcAnimNode->mNumScalingKeys; k++)
			{
				AnimScale srcScale;
				srcScale.time = (float)srcAnimNode->mScalingKeys[k].mTime;
				srcScale.scale.x = (float)srcAnimNode->mScalingKeys[k].mValue.x;
				srcScale.scale.y = (float)srcAnimNode->mScalingKeys[k].mValue.y;
				srcScale.scale.z = (float)srcAnimNode->mScalingKeys[k].mValue.z;
				animNode->scale.emplace_back(srcScale);
			}

			for (UINT k = 0; k < srcAnimNode->mNumPositionKeys; k++)
			{
				AnimPosition srcPosition;
				srcPosition.time = (float)srcAnimNode->mPositionKeys[k].mTime;
				srcPosition.pos.x = (float)srcAnimNode->mPositionKeys[k].mValue.x;
				srcPosition.pos.y = (float)srcAnimNode->mPositionKeys[k].mValue.y;
				srcPosition.pos.z = (float)srcAnimNode->mPositionKeys[k].mValue.z;
				animNode->position.emplace_back(srcPosition);
			}

			for (UINT k = 0; k < srcAnimNode->mNumRotationKeys; k++)
			{
				AnimRotation srcRotation;
				srcRotation.time = (float)srcAnimNode->mRotationKeys[k].mTime;
				srcRotation.quater.x = (float)srcAnimNode->mRotationKeys[k].mValue.x;
				srcRotation.quater.y = (float)srcAnimNode->mRotationKeys[k].mValue.y;
				srcRotation.quater.z = (float)srcAnimNode->mRotationKeys[k].mValue.z;
				srcRotation.quater.w = (float)srcAnimNode->mRotationKeys[k].mValue.w;
				animNode->rotation.emplace_back(srcRotation);
			}

			Anim->mapAnimNode.insert(make_pair(animNode->name, animNode));
		}

		vecAnimations.emplace_back(Anim);
	}
}

void Model::ReadBone()
{
	if (BoneData.isChanging)
	{
		BlendAnimation();
	}

	for (UINT i = 0; i < vecBones.size(); i++)
	{
		BoneData.CurrentBones[i] =
			BoneAnimations[PlayAnimState.Index]->ArrBoneTrans[PlayAnimState.CurrentFrame][i];
		BoneData.NextBones[i] =
			BoneAnimations[PlayAnimState.Index]->ArrBoneTrans[PlayAnimState.NextFrame][i];
	}
	
	BoneData.Bonesweight = PlayAnimState.Weight;


	if (BoneData.isChanging)
	{
		for (UINT i = 0; i < vecBones.size(); i++)
		{
			ChangeBoneData.CurrentBones[i] =
				BoneAnimations[ChangeAnimState.Index]->ArrBoneTrans[ChangeAnimState.CurrentFrame][i];
			ChangeBoneData.NextBones[i] =
				BoneAnimations[ChangeAnimState.Index]->ArrBoneTrans[ChangeAnimState.NextFrame][i];
		}

		ChangeBoneData.Bonesweight = ChangeAnimState.Weight;
	}
}

void Model::MakeBoneAnimation()
{
	map<string, AnimNode*>::iterator iter;
	map<string, UINT>::iterator iter2;

	UINT size = BoneAnimations.size();

	for (UINT i = 0; i < vecAnimations.size(); i++)
	{
		BoneAnimations.emplace_back(new BoneAnimation());

		BoneAnimations[size + i]->Duration = vecAnimations[i]->Duration + 1;
		BoneAnimations[size + i]->name = vecAnimations[i]->name;
		BoneAnimations[size + i]->TickPerSecond = vecAnimations[i]->TickPerSecond;
		BoneAnimations[size + i]->ArrBoneTrans = new Matrix*[(UINT)BoneAnimations[size + i]->Duration];

		Matrix* Nodes = new Matrix[vecNodes.size()];

		for (UINT k = 0; k < BoneAnimations[size + i]->Duration; k++)
		{
			BoneAnimations[size + i]->ArrBoneTrans[k] = new Matrix[vecBones.size()];

			for (UINT j = 0; j < vecNodes.size(); j++)
			{
				Matrix S, R, T;
				Quaternion quter;
				Vector3 pos, scale;
				Matrix W, animation, parent;
				D3DXMatrixIdentity(&W);

				iter = vecAnimations[i]->mapAnimNode.find(vecNodes[j]->Name);
				iter2 = mapBone.find(vecNodes[j]->Name);

				if (iter != vecAnimations[i]->mapAnimNode.end())
				{
					pos = CalcInterpolatedPosition(iter, (float)k, (int)BoneAnimations[size + i]->Duration);
					scale = CalcInterpolatedScaling(iter, (float)k, (int)BoneAnimations[size + i]->Duration);
					quter = CalcInterpolatedRotation(iter, (float)k, (int)BoneAnimations[size + i]->Duration);
				
					D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
					D3DXMatrixRotationQuaternion(&R, &quter);
					D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

					animation = S * R * T;
				}
				else
				{
					animation = vecNodes[j]->Transform;
				}

				int parentIndex = vecNodes[j]->Parent;

				if (parentIndex < 0)
				{
					D3DXMatrixIdentity(&parent);
				}
				else
				{
					parent = Nodes[parentIndex];
				}

				Nodes[j] = animation * parent;

				if (iter2 != mapBone.end())
				{
					int boneIndex = iter2->second;
					BoneAnimations[size + i]->ArrBoneTrans[k][boneIndex]
						= vecBones[boneIndex]->Offset * Nodes[j];
				}
			}
		}

		iter = vecAnimations[i]->mapAnimNode.begin();

		for (iter; iter != vecAnimations[i]->mapAnimNode.end(); iter++)
		{
			iter->second->position.clear();
			iter->second->position.shrink_to_fit();
			iter->second->scale.clear();
			iter->second->scale.shrink_to_fit();
			iter->second->rotation.clear();
			iter->second->rotation.shrink_to_fit();
			SAFE_DELETE(iter->second);
		}

		vecAnimations[i]->mapAnimNode.clear();
		SAFE_DELETE_ARRAY(Nodes);
	}

	vecAnimations.clear();
	vecAnimations.shrink_to_fit();
}

void Model::BlendAnimation()
{
	AnimCurrentBlendTime += DeltaTime;
	BoneData.ChangingWeight += DeltaTime / AnimBlendTime;

	if (AnimCurrentBlendTime >= AnimBlendTime)
	{
		AnimCurrentBlendTime = 0.0f;
		PlayAnimState.CurrentFrame = ChangeAnimState.CurrentFrame;
		PlayAnimState.Index = ChangeAnimState.Index;
		PlayAnimState.isPlaying = ChangeAnimState.isPlaying;
		PlayAnimState.NextFrame = ChangeAnimState.NextFrame;
		PlayAnimState.State = ChangeAnimState.State;
		BoneData.isChanging = false;
		BoneData.ChangingWeight = 0.0f;
	}
}

void Model::destroy()
{
	__super::destroy();
}

void Model::update()
{
	__super::update();
}

void Model::render()
{
	g_DeviceContext->UpdateSubresource(WorldBuffer, 0, NULL, &W, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(0, 1, &WorldBuffer);

	g_DeviceContext->UpdateSubresource(ColorBuffer, 0, NULL, &color, 0, 0);
	g_DeviceContext->PSSetConstantBuffers(0, 1, &ColorBuffer);

	shader->Render();

	if (BoneData.HasAnimation != 0.0f)
	{
		PlayAnimation(PlayAnimState);

		if (BoneData.isChanging != 0.0f)
		{
			PlayAnimation(ChangeAnimState);
		}
	}

	if (BoneData.HasAnimation != 0.0f)
	{
		ReadBone();
	}

	g_DeviceContext->UpdateSubresource(BoneArrayBuffer, 0, NULL, &BoneData, 0, 0);
	g_DeviceContext->VSSetConstantBuffers(4, 1, &BoneArrayBuffer);

	if (BoneData.isChanging != 0.0f)
	{
		g_DeviceContext->UpdateSubresource(ChangeBoneArrayBuffer, 0, NULL, &ChangeBoneData, 0, 0);
		g_DeviceContext->VSSetConstantBuffers(5, 1, &ChangeBoneArrayBuffer);
	}

	UINT stride = sizeof(VERTEX_MODEL);
	UINT offset = 0;

	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	map<string, TexMtl>::iterator mtliter;
	for (UINT i = 0; i < vecMeshes.size(); i++)
	{
		mtliter = mapMaterial.find(vecMeshes[i]->MaterialKey);
		Mtl->mtl = mtliter->second.mt;

		if (Mtl->mtl.HasMap.x != 0.0f)
		{
			mtliter->second.tx.DiffuseTexture->Render(1);
		}
		if (Mtl->mtl.HasMap.y != 0.0f)
		{
			mtliter->second.tx.NormalTexture->Render(2);
		}
		if (Mtl->mtl.HasMap.z != 0.0f)
		{
			mtliter->second.tx.SpecularTexture->Render(3);
		}
		if (Mtl->mtl.HasMap.w != 0.0f)
		{
			mtliter->second.tx.EmissiveTexture->Render(4);
		}

		Mtl->render();

		g_DeviceContext->IASetVertexBuffers(0, 1, &vecMeshes[i]->vertexBuffer, &stride, &offset);
		g_DeviceContext->IASetIndexBuffer(vecMeshes[i]->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		renderState->BeforeRender();

		g_DeviceContext->DrawIndexed(vecMeshes[i]->IndexCount, 0, 0);
		renderState->AfterRender();
	}
}

void Model::releaseNode()
{
	for (UINT i = 0; i < vecNodes.size(); i++)
	{
		SAFE_DELETE(vecNodes[i]);
	}
	vecNodes.clear();
	vecNodes.shrink_to_fit();

	for (UINT i = 0; i < vecBones.size(); i++)
	{
		SAFE_DELETE(vecBones[i]);
	}
}

void Model::PlayAnimation(AnimState & AnimState)
{
	switch (AnimState.State)
	{
	case ANI_ONCEPLAY:
		if (!AnimState.isPlaying) return;
		AnimState.Weight += DeltaTime * BoneAnimations[AnimState.Index]->TickPerSecond;

		if (AnimState.Weight >= 1.0f)
		{
			AnimState.Weight -= 1.0f;
			AnimState.CurrentFrame++;
			AnimState.NextFrame++;

			if (AnimState.NextFrame == BoneAnimations[AnimState.Index]->Duration)
			{
				AnimState.isPlaying = false;
				AnimState.CurrentFrame = 0;
				AnimState.NextFrame = 1;
			}
		}
		break;
	case ANI_LOOPPLAY:
		AnimState.Weight += DeltaTime * BoneAnimations[AnimState.Index]->TickPerSecond;

		if (AnimState.Weight >= 1.0f)
		{
			AnimState.Weight -= 1.0f;
			AnimState.CurrentFrame++;
			AnimState.NextFrame++;

			if (AnimState.NextFrame == BoneAnimations[AnimState.Index]->Duration)
			{
				AnimState.CurrentFrame = 0;
				AnimState.NextFrame = 1;
			}
		}
		break;
	}
}

void Model::ChangeAnimation(int Index, int State, float BlendTime)
{
	if (BoneData.isChanging) return;

	BoneData.isChanging = true;
	PlayAnimState.State = ANI_STOP;

	ChangeAnimState.isPlaying = true;
	ChangeAnimState.Index = Index;
	ChangeAnimState.Weight = 0.0f;
	ChangeAnimState.CurrentFrame = 0;
	ChangeAnimState.NextFrame = 0;
	ChangeAnimState.State = State;

	AnimBlendTime = BlendTime;
}

void Model::SetVertex(Matrix srt)
{
}

Matrix Model::GetBone(string str)
{
	UINT index = mapBone[str];
	Matrix temp;

	{
		float inverseweight = 1.0f - BoneData.Bonesweight;
		temp = BoneData.NextBones[index] * BoneData.Bonesweight
			+ BoneData.CurrentBones[index] * inverseweight;
	}

	if (BoneData.isChanging)
	{
		Matrix temp2;
		float inverseChangeweight = 1.0f - BoneData.ChangingWeight;
		float inverseweight = 1.0f - ChangeBoneData.Bonesweight;
		temp2 = ChangeBoneData.NextBones[index] * ChangeBoneData.Bonesweight +
			ChangeBoneData.CurrentBones[index] * inverseweight;

		temp = temp * inverseChangeweight + temp2 * BoneData.ChangingWeight;
	}

	Matrix mat;
	mat = mapNode[str] * W;

	return mat;
}