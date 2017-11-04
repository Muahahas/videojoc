#pragma once
#include "RenderPipeline.h"
#include "XML\XML.h"
#include "BeginRenderCmd.h"
#include "EndRenderCmd.h"
#include "SetPerFrameConstantsCmd.h"
#include "ApplyTechniquePool.h"
#include "ClearCmd.h"
#include "RenderSceneLayer.h"
#include "DrawQuad.h"
#include "RenderImGUI.h"
#include <map>
#include <functional>
#include "Engine\Engine.h"
#include "SetRasterizerState.h"
#include "SetDepthStencilStateCmd.h"
#include "SetAlphaBlendState.h"
#include "SetRenderTarget.h"
#include "RenderStagedTexture.h"
#include "DeferredShading.h"
#include "CaptureFrameBuffer.h"
#include "DisableAlphaBlend.h"
#include "EnableAlphaBlend.h"
#include "Utils/Logger.h"
#include "UnSetRenderTarget.h"
#include "ApplyFilter.h"
#include "GenerateShadowMaps.h"
#include"GUI/GUIManager.h"
#include"GUI/GUIPosition.h"
#include "RenderGUI.h"
#include "Utils/Logger.h"

#define RENDER_CMD_ENTRY(tag, command_class_name)  { tag, [] { return new  command_class_name();}},
std::map<std::string, std::function<CRenderCmd*(void)>> sComandsFactory =
{
    RENDER_CMD_ENTRY("begin_render", CBeginRenderCmd)
    RENDER_CMD_ENTRY("end_render", CEndRenderCmd)
    RENDER_CMD_ENTRY("set_per_frame_constants", CSetPerFrameConstantsCmd)
    RENDER_CMD_ENTRY("apply_technique_pool", CApplyTechniquePool)
    RENDER_CMD_ENTRY("set_rasterizer_state", CSetRasterizerState)
    RENDER_CMD_ENTRY("set_depth_stencil_state", CSetDepthStencilStateCmd)
    RENDER_CMD_ENTRY("set_alpha_blend_state", CSetAlphaBlendState)
    RENDER_CMD_ENTRY("set_render_target", CSetRenderTarget)
    RENDER_CMD_ENTRY("render_layer", CRenderSceneLayer)
    RENDER_CMD_ENTRY("deferred_shading", CDeferredShading)
    RENDER_CMD_ENTRY("draw_quad", CDrawQuad)
    RENDER_CMD_ENTRY("render_imgui", CRenderImGUI)
    RENDER_CMD_ENTRY("clear", CClearCmd)
    RENDER_CMD_ENTRY("capture_frame_buffer", CCaptureFrameBuffer)
    RENDER_CMD_ENTRY("enable_alpha_blend", CEnableAlphaBlend)
    RENDER_CMD_ENTRY("disable_alpha_blend", CDisableAlphaBlend)
    RENDER_CMD_ENTRY("unset_render_target", CUnSetRenderTarget)
    RENDER_CMD_ENTRY("apply_filter", CApplyFilter)
    RENDER_CMD_ENTRY("generate_shadow_maps", CGenerateShadowMaps)
    RENDER_CMD_ENTRY("render_gui", CRenderGUI)
};

CRenderPipeline::CRenderPipeline() {}

CRenderPipeline::~CRenderPipeline()
{
    CTemplatedMapVector<CRenderCmd>::Destroy();

    // TODO Free memory (and Tibet)
    /*    for (std::map<std::string, std::function<CRenderCmd*(void) >>::iterator itr = sComandsFactory.begin(); itr != sComandsFactory.end(); ++itr)
        {
            delete itr->second();
            sComandsFactory.erase(itr);
        }
    */
}

bool CRenderPipeline::Load(const std::string& aFilename)
{
    bool lOk = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        m_Filename = aFilename;
        CXMLElement* lRenderPipeline = document.FirstChildElement("render_pipeline");

        int lCommandId = 0;

        if (lRenderPipeline)
        {
            for (tinyxml2::XMLElement* iRenderPipeline = lRenderPipeline->FirstChildElement(); iRenderPipeline != nullptr; iRenderPipeline = iRenderPipeline->NextSiblingElement())
            {
                CRenderCmd* lCommand = nullptr;
                LOG_INFO_APPLICATION(iRenderPipeline->Name());
                try
                {
                    lCommand = sComandsFactory[iRenderPipeline->Name()]();
                }
                catch (int e)
                {
                    LOG_ERROR_APPLICATION(iRenderPipeline->Name());
                }

                if (lCommand->Load(iRenderPipeline))
                {
                    Add(std::to_string(lCommandId), lCommand);
                    ++lCommandId;
                }
            }
            lOk = true;
        }
    }

    return lOk;
}

void CRenderPipeline::Execute()
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

    for (size_t i = 0; i < GetCount(); ++i)
    {
        //LOG_INFO_APPLICATION(m_ResourcesVector[i]->GetName().c_str());
        m_ResourcesVector[i]->Execute(lRenderManager);
    }
}

void CRenderPipeline::DrawImgui()
{
    for (size_t i = 0; i < GetCount(); ++i)
    {
        m_ResourcesVector[i]->DrawImgui();
    }
}

void CRenderPipeline::Reload()
{
    Destroy();
    Load(m_Filename);
}
