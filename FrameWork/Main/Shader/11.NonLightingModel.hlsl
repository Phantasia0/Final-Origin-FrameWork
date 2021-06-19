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
	float4 color = DiffuseTexture.Sample(DiffuseSampler, input.Uv);
	color *= MtlDiffuse;
	color.a = saturate(color.a);

	return color;
}