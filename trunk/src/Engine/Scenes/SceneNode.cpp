#include "SceneNode.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "Render/RenderManager.h"
#include "ConstantBufferManager.h"
#include <d3d11.h>


CSceneNode::CSceneNode() {}

CSceneNode::CSceneNode(const CXMLElement* aElement)
    : CName(aElement->GetAttribute<std::string>("name", ""))
    , CTransform((strcmp(aElement->FirstChildElement()->Name(), "transform") == 0) ? aElement->FirstChildElement() : nullptr)
{
    if (strcmp(aElement->FirstChildElement()->Name(), "transform") == 0)
    {
        tinyxml2::XMLElement const *iTransformNode = aElement->FirstChildElement();
        m_PrevPos = iTransformNode->GetAttribute<Vect3f>("forward", Vect3f(0.0f, 0.0f, 1.0f));
    }
}

CSceneNode::~CSceneNode() {}

bool CSceneNode::Update(float aDeltaTime)
{
    return true;
}

bool CSceneNode::Render(CRenderManager& lRM)
{
    Vector4<float> lBSCenter(mBS.GetCenter().x, mBS.GetCenter().y, mBS.GetCenter().z, 0);
    lBSCenter = lRM.GetViewProjectionMatrix() * lBSCenter;
    mBS.SetCenter(Vect3f(lBSCenter.x, lBSCenter.y, lBSCenter.z));

    m_Visible = lRM.m_Frustum->IsVisible(mBS);
    // Todo: Check BS with currentfrustum
    return m_Visible;
}

void CSceneNode::DrawImgui()
{
    if (ImGui::CollapsingHeader(m_Name.c_str()))
    {
        ImGui::SliderFloat3("Position", (float*)&m_Position, -100.0f, 100.0f);
        ImGui::SliderFloat3("Forward", (float*)&m_PrevPos, -100.0f, 100.0f);
        ImGui::SliderFloat3("Scale", (float*)&m_Scale, -100.0f, 100.0f);
    }
}