#include "globals.fx"


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
#endif

#if USE_BUMP
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
#if USE_WEIGHTIDX
		float4 Weight :
			BLENDWEIGHT;
		float4 Indices :
			BLENDINDICES;
#endif
#if UV
	float2 USE_UV :
	    TEXCOORD0;
	#if UV2
		float2 USE_UV2 :
		    TEXCOORD1;
	#endif
#endif
#if USE_BUMP
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
	#if USE_BUMP
		float3 Tangent :
		    TANGENT;
		float3 Binormal :
		    BINORMAL;
	#endif
	#if USE_UV
		float2 UV :
		    TEXCOORD0;
		#if USE_UV2
			float2 UV2 :
			    TEXCOORD1;	

		#endif
	#endif
	float3 WorldNormal :
			    TEXCOORD2;
	float3 WorldPosition :
			    TEXCOORD3;
	float2 Depth : 
				TEXCOORD4;

};



PS_INPUT VS( VS_INPUT IN )
{
    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4( IN.Pos.xyz, 1.0 );
	float3 l_Normal = IN.Normal;
	#if USE_WEIGHTIDX
		float4 l_TempPos=float4(IN.pos.xyz, 1.0);
		l_Normal=float3(0,0,0);	
		lPos=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
		lPos+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
		float3x3 m;
		m[0].xyz = m_Bones[l_Indices.x][0].xyz;
		m[1].xyz = m_Bones[l_Indices.x][1].xyz;
		m[2].xyz = m_Bones[l_Indices.x][2].xyz;
		l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.x;
		m[0].xyz = m_Bones[l_Indices.y][0].xyz;
		m[1].xyz = m_Bones[l_Indices.y][1].xyz;
		m[2].xyz = m_Bones[l_Indices.y][2].xyz;
		l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.y;
		l_Normal=normalize(l_Normal);
	#endif



    l_Output.Pos = mul( lPos, m_World );
    l_Output.WorldPosition=l_Output.Pos.xyz;
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.Depth=l_Output.Pos.zw;
    l_Normal = (l_Normal/2)+0.5;
    l_Output.WorldNormal = normalize(mul(normalize(l_Normal).xyz, (float3x3)m_World));
    #if USE_UV
	 	l_Output.UV = IN.UV;
	 	#if USE_UV2
	 		l_Output.UV2 = IN.UV2;
	 	#endif
    #endif

    #if USE_BUMP
     	l_Output.Tangent = normalize(mul(IN.Tangent.xyz, (float3x3)m_World));
    	l_Output.Binormal = normalize(mul(cross(IN.Tangent.xyz, IN.Normal.xyz), (float3x3)m_World));

    #endif

    return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	
    float g_SpecularExponent = 80.0;
    float g_SpecularContrib = 1.0;
    float3 pixelColor = m_RawData[0].xyz;
    float3 l_LAmbient = m_LightAmbient.xyz;

    float3 l_Normal = normalize(IN.WorldNormal);
    float3 l_WorldPos = IN.WorldPosition;

    float3 l_DiffuseTmp =	float3(0.0, 0.0, 0.0);
    float3 l_SpecularTmp =	float3(0.0, 0.0, 0.0);

    float3 l_LDiffuseSpecular = float3(0.0, 0.0, 0.0);
    float3 l_LDiffuseSpecularTmp = float3(0.0, 0.0, 0.0);
  
	#if USE_BUMP
	 	float3 bump = m_RawData[1].x * (NormalMapTexture.Sample(NormalMapTextureSampler, IN.UV).rgb - float3(0.5, 0.5, 0.5));
	 	l_Normal = l_Normal + bump.x*IN.Tangent + bump.y*IN.Binormal;
    	l_Normal = normalize(l_Normal);
 	#endif 

    #if USE_UV
		pixelColor = (DiffuseTexture.Sample(LinearSampler, IN.UV) * float4(pixelColor, 1.0)).xyz;
		#if USE_UV2
			float4 l_LightmapPixel = LightmapTexture.Sample(LightmapSampler, IN.UV2) * 2;
			l_LAmbient = l_LAmbient * l_LightmapPixel;
			//pixelColor = l_LightmapPixel.xyz * pixelColor;
		#endif
 	#endif

 	
	l_LAmbient = l_LAmbient * pixelColor;
	
 	for(int i = 0; i<MAX_LIGHTS_BY_SHADER; i++)
    {
        CalculateSingleLight(i, l_Normal, l_WorldPos, pixelColor,l_DiffuseTmp, l_SpecularTmp);

        l_LDiffuseSpecularTmp = l_DiffuseTmp + l_SpecularTmp;
        l_LDiffuseSpecular = l_LDiffuseSpecular + l_LDiffuseSpecularTmp;

        l_DiffuseTmp =	float3(0.0, 0.0, 0.0);
        l_SpecularTmp =	float3(0.0, 0.0, 0.0);
    }

    return float4(l_LAmbient+l_LDiffuseSpecular,1.0);

    
}
