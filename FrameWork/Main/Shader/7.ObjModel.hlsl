#include "Header.hlsl"

PI_PTN VS(VI_PTN input)
{
	PI_PTN output;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);
	output.wPosition = output.Position;

	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	output.Uv = input.Uv;

	output.Normal = mul(input.Normal, (float3x3) World);

	return output;
}

float4 PS(PI_PTN input) : SV_TARGET
{

	float3 Light = normalize(DirLight);
	float3 ViewDir = normalize(input.wPosition.xyz - ViewPos.xyz);

	float3 normal = normalize(input.Normal.xyz);

	float d = saturate(dot(-Light, normal));

	float3 reflection = reflect(Light, normal);

	float s = saturate(dot(-ViewDir, reflection));

	s = pow(s, MtlSpecular.a);

	float4 color = Texture.Sample(Sampler, input.Uv);

	color.g = color.r;

	color.b = color.r;

	color *= ColorBuffer;

	float4 result = saturate((color * d * MtlDiffuse)
		+ (color * s * MtlSpecular)
		+ (color * MtlAmbient));

	result.a = MtlAmbient.a * color.a * ColorBuffer.a;

	return result;
}