#include "RenderSceneLayer.h"
#include "XML\XML.h"


CRenderSceneLayer::CRenderSceneLayer()
{

}

CRenderSceneLayer::~CRenderSceneLayer()
{

}

//Leer� el siguiente nodo :
//<render_layer layer="opaque"/>

bool CRenderSceneLayer::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
        m_LayerName = aElement->GetAttribute<std::string>("layer", "opaque");

    return lOk;
}

void CRenderSceneLayer::Execute(CRenderManager& lRM)
{
    //
}