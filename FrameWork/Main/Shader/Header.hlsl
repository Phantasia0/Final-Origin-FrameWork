
struct VI_PC
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
};

struct VI_PT
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
};

struct VI_PS
{
	float4 Position : POSITION0;
	float2 Size : SIZE0;
};

struct VI_PSV
{
	float4 Position : POSITION0;
	float2 Size : SIZE0;
	float3 Velocity : VELOCITY0;
};

struct VI_PCN
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
	float3 Normal : NORMAL0;
};

struct VI_PTN
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL0;
};

struct VI_MODEL
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL0;
	float3 tangent : TANGENT0;
	float4 indices : INDICES0;
	float4 weights : WEIGHTS0;
};

struct PI_PC
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
};

struct PI_PCN
{
	float4 Position : SV_POSITION;
	float4 wPosition : POSITION0;
	float4 Color : COLOR0;
	float3 Normal : NORMAL0;
};

struct PI_PTN
{
	float4 Position : SV_POSITION;
	float4 wPosition : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL0;
};

struct PI_MODEL
{
	float4 Position : SV_POSITION;
	float4 wPosition : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
};

cbuffer VS_World : register(b0)
{
	row_major matrix World;
}

cbuffer VS_View : register(b1)
{
	row_major matrix View;
}

cbuffer VS_Projection : register(b2)
{
	row_major matrix Proj;
}

cbuffer VS_Ortho : register(b3)
{
	row_major matrix Ortho;
}

cbuffer VS_UV : register(b4)
{
	float2 MinUV;
	float2 MaxUV;
}

#define MAX_BONE 256
cbuffer VS_Bones : register(b4)
{
	row_major matrix    CurrentBones[MAX_BONE];
	row_major matrix    NextBones[MAX_BONE];
	float               Bonesweight;
	float               HasAnimation;
	float               isChanging;
	float               ChangingWeight;
}

cbuffer ChangeBones : register(b5)
{
	row_major matrix    cCurrentBones[MAX_BONE];
	row_major matrix    cNextBones[MAX_BONE];
	float               cBonesweight;
	float               cHasAnimation;
	float               cisChanging;
	float               cChangingWeight;
}

cbuffer PS_Color : register(b0)
{
	float4 ColorBuffer;
}

cbuffer PS_Mtl : register(b1)
{
	float4 MtlAmbient;
	float4 MtlDiffuse;
	float4 MtlSpecular;
	float4 MtlEmissive;
	float4 HasMap;
}

cbuffer PS_ViewPos : register(b2)
{
	float4 ViewPos;
}

cbuffer PS_DirLight : register(b4)
{
	float3 DirLight;
	float  Padding;
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

SamplerState DiffuseSampler : register(s1);
Texture2D DiffuseTexture : register(t1);

SamplerState NormalSampler : register(s2);
Texture2D NormalTexture : register(t2);

SamplerState SpecularSampler : register(s3);
Texture2D SpecularTexture : register(t3);

SamplerState EmissiveSampler : register(s4);
Texture2D EmissiveTexture : register(t4);