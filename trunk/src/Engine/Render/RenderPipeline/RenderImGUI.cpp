// si estas capturando mouse o keyboard
//ImGui::GetIO().WantCaptureMouse;

#include "RenderImGUI.h"
#include "ImGUI\imgui.h"

#include "RenderSceneLayer.h"

#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Effects/ShaderManager.h"
#include "Graphics/Effects/EffectManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Graphics/Cinematics/CinematicsManager.h"
#include "Render/RenderPipeline/SetRasterizerState.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Camera/CameraManager.h"

CRenderImGUI::CRenderImGUI() {}
CRenderImGUI::~CRenderImGUI() {}

bool CRenderImGUI::Load(const CXMLElement* aElement)
{
    return CRenderCmd::Load(aElement);
}

void CRenderImGUI::Execute(CRenderManager& lRM)
{
    CEngine& lEngine = CEngine::GetInstance();

    static bool show_app_auto_resize = true;
    ImGui::Begin("Menu", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

    //FPS
    ImGui::Text("%.1f FPS", lEngine.m_FPS);
    if (ImGui::Button("PLAY"))
    {
        lEngine.GetCinematicManager().Play("Animation01");
    }

    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    // #TODO el contador de fps de imgui es bueno?

    // TECHNIQUES
    if (ImGui::CollapsingHeader("Cameras"))
    {
        // CAMERA SELECTION
        ImGui::RadioButton("TPS", &lEngine.GetCameraManager().m_CameraSelector, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Free", &lEngine.GetCameraManager().m_CameraSelector, 1);
    }

    // RELOADS
    Reloads(lEngine);

    //SCENE MANAGEMENT
    lEngine.DrawImgui();

    ImGui::End();
    ImGui::Render();
}

void CRenderImGUI::SceneManager(CEngine& lEngine)
{
    if (ImGui::CollapsingHeader("Scenes Manager"))
    {
        ImGui::BeginChild("#Scenes", ImVec2(400, 200), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-130);

        std::vector<CScene*> scenes = lEngine.GetSceneManager().GetScenes();

        for (std::vector<CScene*>::iterator iScene = scenes.begin(); iScene != scenes.end(); ++iScene)
        {
            ImGui::PushID((*iScene)->GetName().c_str());

            if (ImGui::CollapsingHeader((*iScene)->GetName().c_str()))
            {
                bool lSceneActive = (*iScene)->GetActive();
                ImGui::Checkbox("Active", &lSceneActive);

                if (lSceneActive)
                {
                    ImGui::BeginChild("#Layer", ImVec2(400, 200), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                    ImGui::PushItemWidth(-130);

                    std::vector<CLayer*> layers = (*iScene)->GetLayers();
                    for (std::vector<CLayer*>::iterator iLayer = layers.begin(); iLayer != layers.end(); ++iLayer)
                    {
                        ImGui::PushID((*iLayer)->GetName().c_str());
                        static bool show_app_auto_resize = true;

                        ImGui::Begin("Layer", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

                        //llayer->DrawImgui();

                        ImGui::End();
                        ImGui::PopID();
                    }
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                }
            }
            ImGui::PopID();
        }

        ImGui::PopItemWidth();
        ImGui::EndChild();
    }
}

void CRenderImGUI::Reloads(CEngine& lEngine)
{
    // TECHNIQUES
    if (ImGui::CollapsingHeader("Reload"))
    {
        //SHADERS
        ImGui::PushID(RELOAD_SHADER_BUTTON_ID);
        ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GREEN_HOVER);

        if (ImGui::Button("Shaders"))
        {
            lEngine.GetShaderManager().Reload();
            lEngine.GetEffectManager().Refresh();
        }

        ImGui::PopStyleColor(2);
        ImGui::PopID();

        //ESCENA
        ImGui::PushID(RELOAD_SHADER_BUTTON_ID);
        ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GREEN_HOVER);

        if (ImGui::Button("Scene"))
        {
            lEngine.GetSceneManager().Reload();
            //lEngine.GetEffectManager().Refresh();
        }

        ImGui::PopStyleColor(2);
        ImGui::PopID();


        //LUCES
        ImGui::PushID(RELOAD_LIGHTS_BUTTON_ID);
        ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GREEN_HOVER);

        if (ImGui::Button("Luces"))
        {
            lEngine.GetLightManager().Load(true);
            //lEngine.GetSceneManager().Refresh();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        //RENDER PIPELINE
        ImGui::PushID(RELOAD_RENDERPIPELINE_BUTTON_ID);
        if (ImGui::Button("RenderPipeline"))
            lEngine.GetRenderPipeline().Reload();
        ImGui::PopID();
    }
}