#include "header.hlsl"

cbuffer PS_PostEffect : register(b10)
{
	int count;
	float width;
	float height;
	int select;
}

float4 CrossBlur(float2 uv)
{
	float4 color = 0;

	for (int i = 0; i < count; i++)
	{
		float divX = (1 + i) / width;
		float divY = (1 + i) / height;

		color += Texture.Sample(Sampler, float2(uv.x + divX, uv.y));
		color += Texture.Sample(Sampler, float2(uv.x - divX, uv.y));
		color += Texture.Sample(Sampler, float2(uv.x, uv.y + divY));
		color += Texture.Sample(Sampler, float2(uv.x, uv.y - divY));
	}

	color /= count * 4;

	return color;
}

static const float2 arr[8] =
{
	float2(-1, -1), float2(0, -1), float2(1, -1),
	float2(-1, 0), float2(1, 0),
	float2(-1, 1), float2(0, 1), float2(1, 1)
};

float4 OctaBlur(float2 uv)
{
	float4 color = 0;

	for (int i = 0; i < count; i++)
	{
		float divX = (1 + i) / width;
		float divY = (1 + i) / height;

		for (int j = 0; j < 8; j++)
		{
			float x = arr[j].x * divX + uv.x;
			float y = arr[j].y * divY + uv.y;

			color += Texture.Sample(Sampler, float2(x, y));
		}
	}

	color /= count * 8;

	return color;
}

static const float weights[13] =
{
	0.0561f, 0.1353f, 0.2780f, 0.4868f, 0.7261f, 0.9231f,
	1.0f,
	0.9231f, 0.7261f, 0.4868f, 0.2780f, 0.1353f, 0.0561f
};

float4 GaussianBlur(float2 uv)
{
	float divX = 1.0f / width;
	float divY = 1.0f / height;

	float sum = 0;
	float4 color = 0;

	for (int i = -6; i <= 6; i++)
	{
		float2 temp = uv + float2(divX * i * count, 0);
		color += weights[6 + i] * Texture.Sample(Sampler, temp);

		temp = uv + float2(0, divY * i * count);
		color += weights[6 + i] * Texture.Sample(Sampler, temp);

		sum += weights[6 + i] * 2;
	}

	color /= sum;

	return color;
}

float4 Mosaic(float2 uv)
{
	int x = (int)(uv.x * count);
	int y = (int)(uv.y * count);

	float2 temp;
	temp.x = (float)x / count;
	temp.y = (float)y / count;

	return Texture.Sample(Sampler, temp);
}

float4 RadialBlur(float2 uv)
{
	float2 radiusUV = uv - float2(0.5f, 0.5f);
	float r = length(radiusUV);
	radiusUV /= r;

	r = saturate(2 * r / width);

	float2 delta = -radiusUV * r * r * height / count;

	float4 color = 0;

	for (int i = 0; i < count; i++)
	{
		color += Texture.Sample(Sampler, uv);
		uv += delta;
	}

	color /= count;

	return color;
}

float4 Outline(float2 uv)
{
	float sum = 0;

	for (int i = 1; i <= count; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			float2 temp = float2(1.0f / width * i, 1.0f / height * i);
			temp *= arr[j];

			float4 albedo = Texture.Sample(Sampler, uv + temp);

			sum += albedo.a;
		}
	}

	sum /= count * 8;

	if (sum > 0.0f && sum < 1.0f)
		return float4(1.0f, 1.0f, 0.0f, 1.0f);

	return Texture.Sample(Sampler, uv);
}

float4 Water(float2 uv)
{
	uv.y = uv.y + (sin(uv.y * count) * 0.03);

	float4 Color = Texture.Sample(Sampler, uv);

	return Color;
}

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
	if (select == 1)
		return CrossBlur(input.UV);
	else if (select == 2)
		return OctaBlur(input.UV);
	else if (select == 3)
		return GaussianBlur(input.UV);
	else if (select == 4)
		return Mosaic(input.UV);
	else if (select == 5)
		return RadialBlur(input.UV);
	else if (select == 6)
		return Outline(input.UV);
	else if (select == 7)
		return Water(input.UV);

	float4 TextureColor = Texture.Sample(Sampler, input.UV);

	return TextureColor;
}