#include "Samplers.fxh"
#include "globals.fx"

struct PS_INPUT
{
float4 Pos :
    SV_POSITION;
float2 UV :
    TEXCOORD0;
};


static bool m_Active=m_RawData[4].x;
static float m_Exposure=m_RawData[5].x;
static float m_Decay=m_RawData[6].x;
static float m_Density=m_RawData[7].x;
static float m_Weight=m_RawData[8].x;
static int m_NumSamples=(int)m_RawData[9].x;
static float2 m_LightScatteringPosition=float2(m_RawData[10].x, m_RawData[10].y);


void Calc2DLinearFunction(float2 PosA, float2 PosB, out float m, out float b)
{
	float Ax=PosB.x-PosA.x;
	float Ay=PosB.y-PosA.y;
	m=Ay/Ax;
	b=PosA.y-m*PosA.x;
}

float CalcPosY2DLinearFunction(float x, float m, float b)
{
	return m*x+b;
}

float CalcPosX2DLinearFunction(float y, float m, float b)
{
	return (y-b)/m;
}

float2 ClampLightScatteringPosition(float2 UV, float2 LightScatteringPosition)
{
	float2 l_LightScatteringPosition=LightScatteringPosition;
	float l_mLinearFunction;
	float l_bLinearFunction;
	Calc2DLinearFunction(UV, LightScatteringPosition, l_mLinearFunction, l_bLinearFunction);
	float l_LightScatteringPositionRight=CalcPosY2DLinearFunction(1.0, l_mLinearFunction, l_bLinearFunction);
	float l_LightScatteringPositionLeft=CalcPosY2DLinearFunction(0.0, l_mLinearFunction, l_bLinearFunction);
	float l_LightScatteringPositionTop=CalcPosX2DLinearFunction(0.0, l_mLinearFunction, l_bLinearFunction);
	float l_LightScatteringPositionBottom=CalcPosX2DLinearFunction(1.0, l_mLinearFunction, l_bLinearFunction);
	float4 l_Color=float4(1,1,1,1);

	if(l_LightScatteringPosition.x<0.0 || l_LightScatteringPosition.y<0.0 || l_LightScatteringPosition.x>1.0 || l_LightScatteringPosition.y>1.0)
	{
		if(l_LightScatteringPositionRight>=0 && l_LightScatteringPositionRight<=1.0 && LightScatteringPosition.x>=UV.x)
			l_LightScatteringPosition=float2(1.0, l_LightScatteringPositionRight);
		else if(l_LightScatteringPositionLeft>=0 && l_LightScatteringPositionLeft<=1.0 && LightScatteringPosition.x<UV.x)
			l_LightScatteringPosition=float2(0.0, l_LightScatteringPositionLeft);
		else if(l_LightScatteringPositionTop>=0 && l_LightScatteringPositionTop<=1.0 && LightScatteringPosition.y<UV.y)
			l_LightScatteringPosition=float2(l_LightScatteringPositionTop, 0.0);
		else if(l_LightScatteringPositionBottom>=0 && l_LightScatteringPositionBottom<=1.0 && LightScatteringPosition.y>=UV.y)
			l_LightScatteringPosition=float2(l_LightScatteringPositionBottom, 1.0);
	}
	return l_LightScatteringPosition;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	if(!m_Active)
		return float4(0,0,0,0);

	float2 l_LightScatteringPosition=ClampLightScatteringPosition(IN.UV, m_CameraPositionInScreen.xy); //m_LightScatteringPosition);
	float2 l_DeltaTextCoord=IN.UV-l_LightScatteringPosition;
	float2 l_TextCoo=IN.UV;
	l_DeltaTextCoord *= 1.0/float(m_NumSamples) * m_Density;
	float l_IlluminationDecay=1.0;
	float4 l_Color=float4(0,0,0,0);
	for(int i=0; i < m_NumSamples ; i++)
	{
		l_TextCoo -= l_DeltaTextCoord;
		float4 l_Sample=T0Texture.SampleGrad(S0Sampler, l_TextCoo, 0, 0);
		l_Sample*=l_IlluminationDecay*m_Weight;
		l_Color+=l_Sample;
		l_IlluminationDecay*=m_Decay;
	}
	//return float4(1,0,0,1);
	return float4(l_Color.xyz*m_Exposure, 0.0);
}