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

PI_MODEL VS(VI_MODEL input)
{
	PI_MODEL output;
	output.Position = input.Position;

	row_major matrix boneWorld = World;

	if (HasAnimation != 0.0f)
	{
		boneWorld = mul(SkinWorld(input.indices, input.weights), World);
	}

	output.Position = mul(output.Position, boneWorld);
	output.wPosition = output.Position;

	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	output.Uv = input.Uv;

	output.Normal = mul(input.Normal, (float3x3) World);
	output.Tangent = mul(input.tangent, (float3x3) World);

	return output;
}

float4 PS(PI_MODEL input) : SV_TARGET
{
	float4 NormalMap;
	float3 Normal;

	if (HasMap.y != 0.0f)
	{
		NormalMap = NormalTexture.Sample(NormalSampler, input.Uv);
		float3 bump = 0;
		TangentSpace(bump, NormalMap.rgb, input.Normal, input.Tangent);
		Normal = bump;
	}
	else
	{
		Normal = input.Normal.xyz;
	}

	float3 Light = normalize(DirLight);
	float3 ViewDir = normalize(input.wPosition.xyz - ViewPos.xyz);

	Normal = normalize(Normal);

	float d = saturate(dot(-Light, Normal));

	float3 reflection = reflect(Light, Normal);

	float s = saturate(dot(-ViewDir, reflection));

	s = pow(s, MtlSpecular.a);

	float4 color;

	if (HasMap.x != 0.0f)
	{
		color = DiffuseTexture.Sample(DiffuseSampler, input.Uv);
	}

	float3 result = saturate((color.rgb * d * MtlDiffuse.rgb)
		+ (color.rgb * s * MtlSpecular.rgb)
		+ (color.rgb * MtlAmbient.rgb));

	float Alpha = saturate(MtlAmbient.a * color.a);

	return float4(result, Alpha);
}