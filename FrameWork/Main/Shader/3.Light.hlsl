#include "Header.hlsl"

PI_PCN VS(VI_PCN input)
{
	PI_PCN output;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);
	output.wPosition = output.Position;

	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	output.Color = input.Color;

	output.Normal = mul(input.Normal, (float3x3) World);

	return output;
}

float4 PS(PI_PCN input) : SV_TARGET
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

	return result;
}