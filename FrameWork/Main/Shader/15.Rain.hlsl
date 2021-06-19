struct VI_PS
{
	float4 Position : POSITION0;
	float2 Size : SIZE0;
};

cbuffer VS_Data : register(b10)
{
	float3 velocity;
	float padding1;

	float4 color;
	float3 origin;
	float time;

	float3 size;
	float padding2;
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

struct VertexOutput
{
	float3 position : POSITION0;
	float2 size : SIZE0;
	float4 color : COLOR0;
	float3 velocity : VELOCITY0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : UV0;
	float4 color : COLOR0;
};

VertexOutput VS(VI_PS input)
{
	VertexOutput output;

	float3 v = velocity;

	v.xz /= input.Size.y * 0.1f;

	output.velocity = v;

	float3 displace = time * v;

	input.Position.xyz = origin.xyz +
	(size.xyz + (input.Position.xyz + displace.xyz) % size.xyz) % size.xyz - (size.xyz * 0.5f);

	output.color = color;
	output.position = input.Position.xyz;
	output.size = input.Size;

	return output;
}

static const float2 TEXCOORD[4] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> stream)
{
	float3 up = normalize(-input[0].velocity);

	float3 forward = ViewPos.xyz - input[0].position;
	forward = normalize(forward);

	float3 right = normalize(cross(up, forward));

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
		output.color = input[0].color;

		stream.Append(output);
	}
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 result = Texture.Sample(Sampler, input.uv);
	result *= input.color;

	return result;
}