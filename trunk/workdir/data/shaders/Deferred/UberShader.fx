#include "Colors.fxh"
#include "globals.fx"


#if UV
Texture2D DiffuseTexture :
register(t0);
SamplerState LinearSampler :
register(s0);

#if UV2
Texture2D LightmapTexture :
register(t1);
SamplerState LightmapSampler :
register(s1);
#endif
#endif

#if BUMP
Texture2D NormalMapTexture :
register(t1);
SamplerState NormalMapTextureSampler :
register(s1);
#endif

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;
#if UV
	float2 UV :
	    TEXCOORD0;
	#if UV2
		float2 UV2 :
		    TEXCOORD1;
	#endif
#endif
#if BUMP
	float3 Tangent:
	    TANGENT;
	float3 Binormal:
	    BINORMAL;

#endif

};


struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
#if BUMP
	float3 Tangent :
	    TANGENT;
	float3 Binormal :
	    BINORMAL;
#endif
#if UV
	float2 UV :
	    TEXCOORD0;
	#if UV2
		float2 UV2 :
		    TEXCOORD1;
		float3 WorldNormal :
		    TEXCOORD2;
		float3 WorldPosition :
		    TEXCOORD3;
		float2 Depth : 
			TEXCOORD4;
	#else
		float3 WorldNormal :
		    TEXCOORD1;
		float3 WorldPosition :
		    TEXCOORD2;
		float2 Depth : 
			TEXCOORD3;
	#endif
#else
	float3 WorldNormal :
	    TEXCOORD0;
	float3 WorldPosition :
	    TEXCOORD1;
	float2 Depth : 
		TEXCOORD2;
#endif


};

struct PixelOutputType
{
float4 Target0 :
    SV_Target0; // albedo
float4 Target1 : 
    SV_Target1; // normal
float4 Target2 : 
    SV_Target2; // Lightning
float4 Target3 :
    SV_Target3; // depth

PS_INPUT VS( VS_INPUT IN )
{
    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4( IN.Pos.xyz, 1.0 );
    l_Output.Pos = mul( lPos, m_World );
    l_Output.WorldPosition=l_Output.Pos.xyz;
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.Depth=l_Output.Pos.zw;

    l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
    #if UV
	 	l_Output.UV = IN.UV;
	 	#if UV2
	 		l_Output.UV2 = IN.UV2;
	 	#endif
    #endif

    #if BUMP
     	l_Output.Tangent = normalize(mul(IN.Tangent.xyz, (float3x3)m_World));
    	l_Output.Binormal = normalize(mul(cross(IN.Tangent.xyz, IN.Normal.xyz), (float3x3)m_World));

    #endif

    return l_Output;
}

PixelOutputType PS(PS_INPUT IN) : SV_Target
{

   
    float g_SpecularExponent = 80.0;
    float g_SpecularContrib = 1.0;

    float3 pixelColor = m_RawData[0].xyz;
    float3 l_LAmbient = m_LightAmbient.xyz;

    float3 l_WorldPos = IN.WorldPosition;
    float3 l_Normal = normalize(IN.WorldNormal);

    float l_Depth=IN.Depth.x/IN.Depth.y;

    PixelOutputType l_Output = (PixelOutputType)0;
    #if UV
		pixelColor = (DiffuseTexture.Sample(LinearSampler, IN.UV) * float4(pixelColor, 1.0)).xyz;
		#if UV2
			float4 l_LightmapPixel = LightmapTexture.Sample(LightmapSampler, IN.UV2) * 2;
			//pixelColor = l_LightmapPixel.xyz * pixelColor;
			l_LAmbient = l_LAmbient * l_LightmapPixel.xyz;
		#endif
 	#endif
 	#if BUMP
	 	float3 bump = m_RawData[1].x * (NormalMapTexture.Sample(NormalMapTextureSampler, IN.UV).rgb - float3(0.5, 0.5, 0.5));
	 	l_Normal = l_Normal + bump.x*IN.Tangent + bump.y*IN.Binormal;
    	l_Normal = normalize(l_Normal);
 	#endif

//  CalculateSingleLight(i, l_Normal, l_WorldPos, pixelColor,l_DiffuseTmp, l_SpecularTmp);

    l_Output.Target0 = float4(pixelColor, g_SpecularContrib);
    l_Output.Target1 = float4(m_LightAmbient.xyz*pixelColor, g_SpecularExponent); 
    l_Output.Target2 = float4(l_Normal, 0.0);	
    l_Output.Target3 = float4(l_Depth, l_Depth, l_Depth, 1.0);

    return l_Output;

    
}
