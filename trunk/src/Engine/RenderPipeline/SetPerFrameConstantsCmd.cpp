#include "SetPerFrameConstantsCmd.h"
#include "XML\XML.h"


CSetPerFrameConstantsCmd::CSetPerFrameConstantsCmd()
{

}

CSetPerFrameConstantsCmd::~CSetPerFrameConstantsCmd()
{

}

//Esta clase realizar� el set de las matrices de view y projection de la c�mara activa.
//Leer� el siguiente nodo de xml: <set_per_frame_constants name="set_per_frame_cmd"/>


bool CSetPerFrameConstantsCmd::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    return lOk;
}

void CSetPerFrameConstantsCmd::Execute(CRenderManager& lRM)
{
    lRM.SetModelMatrix(lRM.m_ModelMatrix);
    lRM.SetProjectionMatrix(lRM.m_ProjectionMatrix);
}
