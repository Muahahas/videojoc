#include "globals.fx"
#include "Samplers.fxh"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};

static float m_ZBlurActive = 1.0; //m_RawDataValues[0];
static float m_ZBlurShowDepths = 1.0; // m_RawDataValues[1];
static float m_ZBlurConstantBlur = 0.3; // m_RawDataValues[2];
static float m_ZBlurFocalStart = 0.1; // m_RawDataValues[3];
static float m_ZBlurFocalEnd = 0.5; //m_RawDataValues[4];
static float m_ZBlurEnd = 1.0; // m_RawDataValues[5];

float4 GetZBlurColor(float Distance, float4 SourceColor, float4 BlurColor)
{
    float l_Blur=1.0;

    if(Distance < m_ZBlurFocalStart)
        l_Blur=max(Distance / m_ZBlurFocalStart, m_ZBlurConstantBlur);
    else if(Distance > m_ZBlurFocalEnd)
        l_Blur=max(1.0-(Distance-m_ZBlurFocalEnd)/m_ZBlurEnd, m_ZBlurConstantBlur);

    if(l_Blur<1.0)
        return BlurColor*(1-l_Blur)+l_Blur*SourceColor;
    else
        return SourceColor;
}

float4 ZBlurPS(PS_INPUT IN) : SV_Target
{
    float4 l_SourceColor=T0Texture.Sample(S0Sampler, IN.UV);
	float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).r;
    float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	float3 l_CameraPosition=m_InverseView[3].xyz;
    float l_Distance=length(l_WorldPosition-l_CameraPosition);	
    return GetZBlurColor(l_Distance, l_SourceColor, T1Texture.Sample(S1Sampler, IN.UV));
}
