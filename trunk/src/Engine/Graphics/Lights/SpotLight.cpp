#include "SpotLight.h"
#include "ImGui/imgui.h"
#include "XML\xml.h"
#include "Engine\Engine.h"
#include "Graphics\Buffers\ConstantBufferManager.h"


CSpotLight::~CSpotLight() {}

CSpotLight::CSpotLight(const CXMLElement* aElement)
    : CLight(aElement)
    , m_Angle(aElement->GetAttribute<float>("angle",45.0f))
    , m_FallOff(aElement->GetAttribute<float>("fall_off",45.0f))
{}


void CSpotLight::SetShadowMap(CRenderManager &RM)
{
    if (m_pShadowMap == NULL)
        return;
    m_ViewShadowMap.SetIdentity();
    m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + m_PrevPos, v3fY);
    unsigned int l_ShadowMapWidth = m_pShadowMap->GetSize().x;
    unsigned int l_ShadowMapHeight = m_pShadowMap->GetSize().y;
    m_ProjectionShadowMap.SetIdentity();
    m_ProjectionShadowMap.SetFromPerspective(m_FallOff, l_ShadowMapWidth / (float)l_ShadowMapHeight, 0.1f, m_RangeAttenuation.y);
    CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();
    RM.SetViewProjectionMatrix(m_ViewShadowMap, m_ProjectionShadowMap);
    lCBM.mFrameDesc.m_View = m_ViewShadowMap;
    lCBM.mFrameDesc.m_Projection = m_ProjectionShadowMap;
    lCBM.mFrameDesc.m_ViewProjection = RM.GetViewProjectionMatrix();
    lCBM.mFrameDesc.m_InverseView = lCBM.mFrameDesc.m_View.GetInverted();
    lCBM.mFrameDesc.m_InverseProjection = lCBM.mFrameDesc.m_Projection.GetInverted();
    lCBM.BindBuffer(RM.GetDeviceContext(), CConstantBufferManager::CB_Frame);
    ID3D11RenderTargetView *l_RenderTargetViews[1];
    l_RenderTargetViews[0] = m_pShadowMap->GetRenderTargetView();
    D3D11_VIEWPORT m_viewport;
    m_viewport.Width = (float)l_ShadowMapWidth;
    m_viewport.Height = (float)l_ShadowMapHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    RM.GetDeviceContext()->RSSetViewports(1, &m_viewport);
    RM.SetRenderTargets(1, l_RenderTargetViews, m_pShadowMap->GetDepthStencilView());
}

void CSpotLight::DrawImgui()
{
    ImGui::SliderFloat("Fall Off Angle",&m_FallOff, 0.0f, 360.0f);
    ImGui::SliderFloat("Angle", &m_Angle, 0.0f, 360.0f);
}

