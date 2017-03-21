#include "Quad.h"
#include "Graphics\Mesh\TemplatedGeometry.h"
#include "Graphics\Mesh\VertexTypes.h"
#include "Engine\Engine.h"

#include "Graphics\Materials\Material.h"
#include "Graphics\Materials\MaterialManager.h"

#include "Graphics\Effects\Effect.h"
#include "Graphics\Effects\PixelShader.h"
#include "Graphics\Effects\VertexShader.h"

/*static CVertexShader sQuadVertexShader(
    "struct PS_INPUT\n"
    "{\n"
    "  float4 Pos : SV_POSITION;\n"
    "  float2 UV : TEXCOORD0;\n"
    "};\n"
    "PS_INPUT quadVS(uint vI : SV_VERTEXID)"
    "{\n"
    "  PS_INPUT l_Output = (PS_INPUT)0;\n"
    "  l_Output.UV = float2(vI & 1, vI >> 1);\n"
    "  l_Output.Pos = float4((l_Output.UV.x - 0.5f) * 2, -(l_Output.UV.y - 0.5f) * 2, 0, 1);\n"
    "  return l_Output;\n"
    "}\n", VertexTypes::PositionUV::GetVertexFlags());

static bool sQuadVertexShaderLoaded = false;*/

/* VertexTypes::Dummy::GetVertexFlags());
Vertex::Dummy lScreenQuadVertexDummy[4] = { 0,0,0,0 };
CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
CVertexBuffer<Vertex::Dummy> * lVB = new CVertexBuffer<Vertex::Dummy>(lRenderManager, lScreenQuadVertexDummy, 4);
mGeometry = new CGeometryTriangleStrip< Vertex::Dummy >(lVB);

*/
CQuad::CQuad()
    : mGeometry(nullptr)
{}

CQuad::~CQuad() {}

bool CQuad::Init()
{
    bool lOk = false;
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    VertexTypes::PositionUV lScreenVertexsQuad[4] =
    {
        { Vect3f(-1.0f, 1.0f, 0.5f), Vect2f(0.0f, 0.0f) },
        { Vect3f(1.0f, 1.0f, 0.5f), Vect2f(1.0f, 0.0f) },
        { Vect3f(-1.0f, -1.0f, 0.5f), Vect2f(0.0f, 1.0f) },
        { Vect3f(1.0f, -1.0f, 0.5f), Vect2f(1.0f, 1.0f) }
    };

    CVertexBuffer<VertexTypes::PositionUV> * lVB = new CVertexBuffer<VertexTypes::PositionUV>(lRenderManager, lScreenVertexsQuad, 4);
    mGeometry = new CGeometryTriangleStrip< VertexTypes::PositionUV >(lVB);

    return lOk;
}

bool CQuad::Render()
{
    bool lOk = false;

    ID3D11DeviceContext* lContext = CEngine::GetInstance().GetRenderManager().GetDeviceContext();
    lOk = mGeometry->Render(lContext);
    return lOk;
}

bool CQuad::Render(CEffect* EFfect)
{
    ID3D11DeviceContext* lContext = CEngine::GetInstance().GetRenderManager().GetDeviceContext();
    EFfect->Bind(lContext);
    return Render();
}
