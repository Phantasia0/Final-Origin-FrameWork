#include "Header.hlsl"

PI_PC VS(VI_PC input)
{
	PI_PC output;
	output.Position = input.Position;

	output.Position = mul(output.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Proj);

	output.Color = input.Color;

	return output;
}

float4 PS(PI_PC input) : SV_TARGET
{
	return  input.Color * ColorBuffer;
}