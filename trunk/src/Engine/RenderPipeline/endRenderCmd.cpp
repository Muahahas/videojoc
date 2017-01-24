#include "EndRenderCmd.h"
#include "XML\XML.h"


CEndRenderCmd::CEndRenderCmd()
{

}

CEndRenderCmd::~CEndRenderCmd()
{

}

//Leer� el siguiente nodo del xml: <end_render name="end_render_main_loop"/>

bool CEndRenderCmd::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    return lOk;
}

void CEndRenderCmd::Execute(CRenderManager& lRM)
{
    lRM.EndRender();
}