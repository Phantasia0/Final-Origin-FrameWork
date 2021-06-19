#include "header.hlsl"

struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 UV : UV0;
};

PixelInput VS(VI_PT input)
{
	PixelInput output = (PixelInput) 0;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);

	output.UV = input.Uv;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 TextureColor = Texture.Sample(Sampler, input.UV);

	float4 Mono;
	Mono.a = TextureColor.a;

	float4 sepia;
	sepia.a = TextureColor.a;

	sepia.r = dot(TextureColor.rgb, float3(0.393f, 0.769f, 0.189f));
	sepia.g = dot(TextureColor.rgb, float3(0.349f, 0.686f, 0.168f));
	sepia.b = dot(TextureColor.rgb, float3(0.272f, 0.534f, 0.131f));

	TextureColor = sepia;

	return TextureColor;
}