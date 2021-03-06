#include "globals.fx"
#include "Samplers.fxh"

struct VS_INPUT
{
float3 Pos :
    POSITION;
float2 UV :
    TEXCOORD0;
};

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};


static float m_NoisePct = m_RawData[0].x; //0.2;
static float m_VignettingPct = m_RawData[1].x; //0.5;
static float m_NoiseAmount = m_RawData[2].x; //0.2;

PS_INPUT VS(VS_INPUT IN)
{
    PS_INPUT l_Output = (PS_INPUT)0;
    l_Output.Pos = float4(IN.Pos, 1.0);
    l_Output.UV = IN.UV;
    return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
    float l_NoiseX = m_Time.x*sin(IN.UV.x*IN.UV.y+m_Time.x);
    l_NoiseX = fmod(l_NoiseX, 8)*fmod(l_NoiseX, 4);
    float l_DistortX = fmod(l_NoiseX, m_NoiseAmount);
    float l_DistortY = fmod(l_NoiseX, m_NoiseAmount+0.002);
    float2 l_DistortUV = float2(l_DistortX, l_DistortY);
    float4 l_Noise = T0Texture.Sample(S0Sampler, IN.UV+l_DistortUV)*m_NoisePct;

    float4 l_Vignetting=T1Texture.Sample(S1Sampler, IN.UV)*m_VignettingPct;
    return l_Noise + l_Vignetting;
}
