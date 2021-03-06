#pragma once
#ifndef _ENGINE_CONSTANTBUFFERMANAGER_20170118_H
#define _ENGINE_CONSTANTBUFFERMANAGER_20170118_H

#define MAXBONES 80
#define MAX_OBJECT_RAW_PARAMETER 16
#define MAX_LIGHTS_BY_SHADER 4

#include "Math/Matrix44.h"
#include "ConstantBuffer.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

struct PerFrameDesc
{
    Mat44f m_View;
    Mat44f m_Projection;
    Mat44f m_ViewProjection;
    Vect4f m_CameraPosition;
    Vect4f m_CameraFrontVector;
    Vect4f m_CameraUpVector;
    Mat44f m_InverseView;
    Mat44f m_InverseProjection;
    Vect4f m_CameraInfo; //x = near, y = far, z = fov, w = aspect ratio
    Vect4f m_TimeSeconds;
    Vect4f m_CameraPositionInScreen;
};

struct PerLightsDesc
{
    Vect4f m_LightAmbient;
    Vect4f m_FogColor; //nuevo
    Vect4f m_FogInfo; //nuevo //x = Enabled, y = Density, z = FogStartRange, w = FogEndRange
    float m_LightEnabled[MAX_LIGHTS_BY_SHADER];
    float m_LightType[MAX_LIGHTS_BY_SHADER];
    Vect4f m_LightPosition[MAX_LIGHTS_BY_SHADER];
    Vect4f m_LightDirection[MAX_LIGHTS_BY_SHADER];
    float m_LightAngle[MAX_LIGHTS_BY_SHADER];
    float m_LightFallOffAngle[MAX_LIGHTS_BY_SHADER];
    float m_LightAttenuationStartRange[MAX_LIGHTS_BY_SHADER];
    float m_LightAttenuationEndRange[MAX_LIGHTS_BY_SHADER];
    float m_LightIntensity[MAX_LIGHTS_BY_SHADER];
    Vect4f m_LightColor[MAX_LIGHTS_BY_SHADER];
    float m_UseShadowMap[MAX_LIGHTS_BY_SHADER]; //nuevo
    float m_UseShadowMask[MAX_LIGHTS_BY_SHADER]; //nuevo
    float m_ShadowMapBias[MAX_LIGHTS_BY_SHADER]; //nuevo
    float m_ShadowMapStrength[MAX_LIGHTS_BY_SHADER]; //nuevo
    Mat44f m_LightView[MAX_LIGHTS_BY_SHADER]; //nuevo
    Mat44f m_LightProjection[MAX_LIGHTS_BY_SHADER]; //nuevo
};

struct PerObjectDesc
{
    Mat44f m_World;
};

struct PerAnimatedModelDesc
{
    Mat44f m_Bones[MAXBONES];
};

struct PerMaterialDesc
{
    Vect4f m_RawData[MAX_OBJECT_RAW_PARAMETER];
    Vect2u m_ViewPosition[MAX_OBJECT_RAW_PARAMETER];
    Vect2u m_ViewSize[MAX_OBJECT_RAW_PARAMETER];

};

class CConstantBufferManager
{
public:
    enum CConstanBufferDEF {CB_Frame, CB_Object,CB_Material, CB_AnimatedModel, CB_Light, NumConstantBuffers};

    PerObjectDesc      mObjDesc;
    PerFrameDesc       mFrameDesc;
    PerAnimatedModelDesc mAnimatedModelDesc;
    PerMaterialDesc mMaterialDesc;
    PerLightsDesc mLightsDesc;

    CConstantBufferManager();
    virtual ~CConstantBufferManager();
    virtual void BindBuffer(ID3D11DeviceContext* aContext, uint32 aConstantBufferID);

protected:
    CConstantBuffer* m_ConstantBuffers[NumConstantBuffers];
};

class CMaterialEffectParameters
{
public:
    Vect4f m_RawData[MAX_OBJECT_RAW_PARAMETER];
};

#endif //_ENGINE_CONSTANTBUFFERMANAGER_20170118_H