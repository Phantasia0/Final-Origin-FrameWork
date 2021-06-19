#include "Header.hlsl"

row_major matrix SkinWorld(float4 indices, float4 weights)
{
	row_major matrix transform = 0;

	transform += mul(weights.x, CurrentBones[(uint) indices.x]);
	transform += mul(weights.y, CurrentBones[(uint) indices.y]);
	transform += mul(weights.z, CurrentBones[(uint) indices.z]);
	transform += mul(weights.w, CurrentBones[(uint) indices.w]);

	row_major matrix transform2 = 0;

	transform2 += mul(weights.x, NextBones[(uint) indices.x]);
	transform2 += mul(weights.y, NextBones[(uint) indices.y]);
	transform2 += mul(weights.z, NextBones[(uint) indices.z]);
	transform2 += mul(weights.w, NextBones[(uint) indices.w]);

	transform = transform * (1.0f - Bonesweight) + transform2 * Bonesweight;

	if (isChanging != 0.0f)
	{
		row_major matrix transform3 = 0;

		transform3 += mul(weights.x, cCurrentBones[(uint) indices.x]);
		transform3 += mul(weights.y, cCurrentBones[(uint) indices.y]);
		transform3 += mul(weights.z, cCurrentBones[(uint) indices.z]);
		transform3 += mul(weights.w, cCurrentBones[(uint) indices.w]);

		row_major matrix transform4 = 0;

		transform4 += mul(weights.x, cNextBones[(uint) indices.x]);
		transform4 += mul(weights.y, cNextBones[(uint) indices.y]);
		transform4 += mul(weights.z, cNextBones[(uint) indices.z]);
		transform4 += mul(weights.w, cNextBones[(uint) indices.w]);

		transform3 = lerp(transform3, transform4, cBonesweight);

		transform = lerp(transform, transform3, ChangingWeight);
	}

	return transform;
}

void TangentSpace(inout float3 bump, float3 normalMap, float3 normal, float3 tangent)
{
	float3 coord = 2.0f * normalMap - 1.0f;

	float3 N = normal;
	float3 T = tangent;
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	bump = mul(coord, TBN);
}

struct PI_INPUT
{
	float4 Position : SV_POSITION;
};

PI_INPUT VS(VI_MODEL input)
{
	PI_INPUT output;
	output.Position = input.Position;

	row_major matrix boneWorld = World;

	if (HasAnimation != 0.0f)
	{
		boneWorld = mul(SkinWorld(input.indices, input.weights), World);
	}

	output.Position = mul(output.Position, boneWorld);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	return output;
}

float4 PS(PI_INPUT input) : SV_TARGET
{
	float depth = input.Position.z / input.Position.w;

	return float4(depth, depth, depth, 1.0f);
}