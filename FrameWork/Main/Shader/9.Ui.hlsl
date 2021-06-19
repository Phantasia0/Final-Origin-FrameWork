#include "Header.hlsl"

PI_PTN VS(VI_PTN input)
{
	PI_PTN output;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);
	output.wPosition = output.Position;

	output.Position = mul(output.Position, Ortho);

	output.Uv = input.Uv;

	if (output.Uv.x == 0.0f)
	{
		output.Uv.x = MinUV.x;
	}
	else if (output.Uv.x == 1.0f)
	{
		output.Uv.x = MaxUV.x;
	}
	if (output.Uv.y == 0.0f)
	{
		output.Uv.y = MinUV.y;
	}
	else if (output.Uv.y == 1.0f)
	{
		output.Uv.y = MaxUV.y;
	}

	output.Normal = input.Normal;

	return output;
}

float4 PS(PI_PTN input) : SV_TARGET
{
	float4 color = Texture.Sample(Sampler, input.Uv) * ColorBuffer;

	return color;
}