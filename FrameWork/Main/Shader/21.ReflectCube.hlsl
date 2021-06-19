#include "Header.hlsl"

Texture2D ReflectMap : register(t10);
SamplerState ReflectSampler : register(s10);

cbuffer VS_RView : register(b10)
{
	row_major matrix RView;
	row_major matrix RProjection;
}

struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 wPosition : POSITION0;
	float4 Color : COLOR0;
	float3 Normal : NORMAL0;
	float4 vPosition : POSITION1;
};

PixelInput VS(VI_PCN input)
{
	PixelInput output;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);
	output.wPosition = output.Position;

	output.vPosition = mul(output.Position, RView);
	output.vPosition = mul(output.vPosition, RProjection);

	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	output.Color = input.Color;

	output.Normal = mul(input.Normal, (float3x3) World);

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float3 Light = normalize(DirLight);
	float3 ViewDir = normalize(input.wPosition.xyz - ViewPos.xyz);

	float3 normal = input.Normal.xyz;

	normal = normalize(normal);
	
	float d = saturate(dot(-Light, normal));

	float3 reflection = reflect(Light, normal);

	float s = saturate(dot(-ViewDir, reflection));

	s = pow(s, MtlSpecular.a);

	float4 color = input.Color * ColorBuffer;

	float4 result = saturate((color * d * MtlDiffuse)
		+ (color * s * MtlSpecular)
		+ (color * MtlAmbient));

	result.a = ColorBuffer.a;

	input.vPosition.xyz /= input.vPosition.w;

	input.vPosition.x = input.vPosition.x * 0.5f + 0.5f;
	input.vPosition.y = -input.vPosition.y * 0.5f + 0.5f;

	float4 tex = ReflectMap.Sample(ReflectSampler, input.vPosition.xy);

	return float4(tex.rgb, 1.0f);
}