#include "Header.hlsl"

cbuffer VS_LView : register(b10)
{
	row_major matrix LightView;
	row_major matrix LightProjection;
}

cbuffer PS_Shadow : register(b10)
{
	float2 mapSize;
	float bias;
	int selected;
}

Texture2D depthMap : register(t10);
SamplerComparisonState depthSampler: register(s10);

struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 Uv : UV0;
	float4 vPosition : POSITION1;
};

PixelInput VS(VI_PTN input)
{
	PixelInput output;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	output.vPosition = mul(input.Position, World);
	output.vPosition = mul(output.vPosition, LightView);
	output.vPosition = mul(output.vPosition, LightProjection);

	output.Uv = input.Uv;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	input.vPosition.xyz /= input.vPosition.w;
	
	float4 color = Texture.Sample(Sampler, input.Uv);

	if (input.vPosition.x < -1.0f || input.vPosition.x > 1.0f ||
		input.vPosition.y < -1.0f || input.vPosition.y > 1.0f ||
		input.vPosition.z < 0.0f || input.vPosition.z > 1.0f)
	{
		return color;
	}
	else
	{
		input.vPosition.x = input.vPosition.x * 0.5f + 0.5f;
		input.vPosition.y = -input.vPosition.y * 0.5f + 0.5f;

		float depth = depthMap.SampleCmpLevelZero(depthSampler, input.vPosition.xy, input.vPosition.z).r;

		float factor = (float)input.vPosition.z <= depth;

		return float4(color.rgb * factor, 1.0f);
	}
}