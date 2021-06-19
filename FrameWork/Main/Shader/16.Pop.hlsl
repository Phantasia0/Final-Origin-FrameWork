struct VI_PSV
{
	float4 position : POSITION0;
	float2 size : SIZE0;
	float3 velocity : VELOCITY0;
};

cbuffer VS_World : register(b0)
{
	row_major matrix World;
}

cbuffer VS : register(b10)
{
	float duration;
	float age;
	float gravity;
}

cbuffer GS_View : register(b0)
{
	row_major matrix View;
}

cbuffer GS_Projection : register(b1)
{
	row_major matrix Proj;
}

cbuffer GS_ViewPos : register(b2)
{
	float4 ViewPos;
}

cbuffer PS : register(b10)
{
	float4 startColor;
	float4 endColor;
}

struct VertexOutput
{
	float3 position : POSITION0;
	float2 size : SIZE0;
	float time : TIME0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : UV0;
	float time : TIME0;
};

VertexOutput VS(VI_PSV input)
{
	VertexOutput output;

	output.time = age / duration;

	input.velocity.y -= gravity * age;

	output.position = input.position.xyz + (input.velocity * age);

	output.position = mul(output.position, (float3x3) World);

	output.size = input.size;
	
	return output;
}

static const float2 TEXCOORD[4] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
}

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> stream)
{
	float3 up = float3(0, 1, 0);
	float3 forward = ViewPos.xyz - input[0].position;

	forward.y = 0.0f;
	forward = normalize(forward);

	float3 right = normalize(cross(up, forward));

	float halfWidth = input[0].size.x * 0.5f;
	float halfHeight = input[0].size.y * 0.5f;

	float4 vertices[4];
	vertices[0] = float4(input[0].position + halfWidth * right - halfHeight * up, 1.0f);
	vertices[1] = float4(input[0].position + halfWidth * right + halfHeight * up, 1.0f);
	vertices[2] = float4(input[0].position - halfWidth * right - halfHeight * up, 1.0f);
	vertices[3] = float4(input[0].position - halfWidth * right + halfHeight * up, 1.0f);

	PixelInput output;
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		output.position = mul(vertices[i], View);
		output.position = mul(output.position, Proj);

		output.uv = TEXCOORD[i];
		output.time = input[0].time;

		stream.Append(output);
	}
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = Texture.Sample(Sampler, input.uv);

	float4 color = lerp(startColor, endColor, input.time);

	float4 returnColor = albedo * color;
	saturate(returnColor);
	return returnColor;
}