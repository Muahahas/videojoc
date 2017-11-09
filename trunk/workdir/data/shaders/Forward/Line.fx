#include "globals.fx"

struct VS_INPUT
{
float3 Pos :
    POSITION;
float3 Normal :
    NORMAL;

};
struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float3 WorldPosition :
    TEXCOORD2;
float3 Normal :
    NORMAL;
float3 WorldNormal :
    TEXCOORD1;

};


PS_INPUT VS( VS_INPUT IN )
{

    PS_INPUT l_Output = (PS_INPUT)0;
    float4 lPos = float4( IN.Pos.xyz, 1.0 );
	l_Output.Pos = lPos;
    l_Output.WorldPosition=l_Output.Pos.xyz;
    l_Output.Pos = mul( l_Output.Pos, m_View );
    l_Output.Pos = mul( l_Output.Pos, m_Projection );       
	return l_Output;
}

float4 PS( PS_INPUT IN ) : SV_Target
{
    return float4(m_RawData[0].xyz,1.0);
}
