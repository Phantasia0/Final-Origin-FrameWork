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

	output.Normal = input.Normal;

	return output;
}

float4 PS(PI_PTN input) : SV_TARGET
{
	float3 Light = normalize(DirLight);
	float3 Nor = normalize(input.Normal.xyz);

	float d = saturate(dot(-Light, Nor));

	if (d >0.9998)
	{
		return float4(1, 1, 1, 1);
	}

	float4 color = Texture.Sample(Sampler, input.Uv);

	d *= 0.1f;

	return float4((0.3f + d), (0.3f + d), (0.5f + d), 1);
}