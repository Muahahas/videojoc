#include "Plane.h"
#include "Mesh/TemplatedIndexedGeometry.h"
#include "Mesh/VertexTypes.h"
#include "Engine/Engine.h"
#include "Mesh/VertexBuffer.h"
#include "Mesh/IndexBuffer.h"

#include "Materials/Material.h"
#include "Materials/MaterialManager.h"

CPlane::CPlane()
    : CMesh()
{
    mMaterials.resize(1);
    mGeometries.resize(1);

    CMaterialManager& lMM = CEngine::GetInstance().GetMaterialManager();
    mMaterials[0] = lMM("Plane002Material");

    bool lOk = false;
    VertexTypes::PositionNormal lPlaneVertex[4] =
    {
        { Vect3f(-0.5f, 0.0f, 0.5f),    Vect3f(0.0f, 1.0f, 0.0f) },
        { Vect3f(0.5f, 0.0f, 0.5f),     Vect3f(0.0f, 1.0f, 0.0f) },
        { Vect3f(0.5f, 0.0f, -0.5f),    Vect3f(0.0f, 1.0f, 0.0f) },
        { Vect3f(-0.5f, 0.0f, -0.5f),   Vect3f(0.0f, 1.0f, 0.0f) }
    };

    uint16 lIndices[6] = { 0, 3, 1, 3, 2, 1 };
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    CVertexBuffer<VertexTypes::PositionNormal> * lVB = new CVertexBuffer<VertexTypes::PositionNormal>(lRenderManager, lPlaneVertex, 4);
    CIndexBuffer* lIB = new CIndexBuffer(lRenderManager, &lIndices, 6, 16);
    mGeometries[0] = new CIndexedGeometryTriangleList< VertexTypes::PositionNormal >(lVB, lIB);
}

CPlane::~CPlane() {}