#include "globals.fx"
#include "Samplers.fxh"

#if USE_UV
	Texture2D DiffuseTexture :
	register(t0);
	SamplerState LinearSampler :
	register(s0);

	#if USE_UV2
		Texture2D LightmapTexture :
		register(t1);
		SamplerState LightmapSampler :
		register(s1);
	#endif
	#if USE_BUMP
		Texture2D NormalMapTexture :
		register(t1);
		SamplerState NormalMapTextureSampler :
		register(s1);
	#endif
#endif

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
#if USE_WEIGHTIDX
		float4 Weight :
			BLENDWEIGHT;
		float4 Indices :
			BLENDINDICES;
#endif
#if USE_UV
	float2 UV :
	    TEXCOORD0;
	#if USE_UV2
		float2 UV2 :
		    TEXCOORD1;
	#endif
	#if USE_BUMP
		float3 Tangent:
		    TANGENT;
		float3 Binormal:
		    BINORMAL;
	#endif
#endif
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Depth : TEXCOORD0;
};

PS_INPUT VS( VS_INPUT IN )
{   
	PS_INPUT l_Out=(PS_INPUT)0;
	l_Out.Pos=mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Out.Pos=mul(l_Out.Pos, m_View);
	l_Out.Pos=mul(l_Out.Pos, m_Projection);
	l_Out.Depth=l_Out.Pos;
	
	return l_Out;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float l_Depth=IN.Depth.z/IN.Depth.w;
	return float4(l_Depth, l_Depth, l_Depth, 1);
}
