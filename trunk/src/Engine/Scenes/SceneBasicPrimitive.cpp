#include "SceneBasicPrimitive.h"
#include "XML/XML.h"
#include "Engine/Engine.h"
#include "Render\RenderManager.h"
#include "Scenes\ConstantBufferManager.h"
#include "Mesh\Plane.h"
#include "Utils\CheckedDelete.h"

CSceneBasicPrimitive::CSceneBasicPrimitive(CXMLElement* aElement)
    : CSceneMesh(aElement)
{
    if (aElement->GetAttribute<std::string>("type", "") == "plane")
        mMesh = new CPlane();
}

CSceneBasicPrimitive::~CSceneBasicPrimitive()
{
    base::utils::CheckedDelete(mMesh);
}
