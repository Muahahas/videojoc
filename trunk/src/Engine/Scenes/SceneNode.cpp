#include "SceneNode.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "Render/RenderManager.h"

CSceneNode::CSceneNode() {}

CSceneNode::CSceneNode(const CXMLElement* aElement)
    : CName(aElement->GetAttribute<std::string>("name", ""))
    , CTransform((strcmp(aElement->FirstChildElement()->Name(), "transform") == 0) ? aElement->FirstChildElement() : nullptr)
{
    // #ALEX Que hacemos en caso de que el nodo transform no este como toca?
    // #ALEX como es el nodo este?
}

CSceneNode::~CSceneNode() {}

bool CSceneNode::Update(float aDeltaTime)
{
    return false;
}

bool CSceneNode::Render(CRenderManager& aRendermanager)
{
    return false;
}