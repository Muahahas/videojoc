#pragma once
#include "Layer.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "SceneMesh.h"
#include "SceneBasicPrimitive.h"
#include "Render/RenderManager.h"
#include "Engine/Engine.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Animation/AnimatedModelManager.h"
#include "Graphics/Mesh/MeshManager.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Particles/ParticleSystemInstance.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "Graphics/Particles/ParticleManager.h"
#include "Graphics/Particles/ParticleSystemType.h"
#include "Graphics/IA/EnemiesManager.h"
#include "Graphics/IA/NavMesh.h"
#include "Graphics/IA/NavMeshManager.h"
//#include "Graphics/IA/Laser.h"
//#include "Graphics/IA/LaserManager.h"

#ifdef _DEBUG
#include <chrono>
#include "Utils/Logger.h"
#include "Utils/MemLeaks/MemLeaks.h"
#endif

CLayer::CLayer(const std::string& aName) :
    CName(aName),
    CActive(false)
{}

CLayer::~CLayer()
{
    CTemplatedMapVector<CSceneNode>::Destroy();
}

bool CLayer::Load(CXMLElement* aElement, bool update)
{
    bool lOk = true;
    CLightManager &lLM = CEngine::GetInstance().GetLightManager();
    CEnemiesManager &lEnemiesManager = CEngine::GetInstance().GetEnemiesManager();
    CLaserManager &l_LaserManager = CEngine::GetInstance().GetLaserManager();
    CNavMeshManager &l_NavMeshManager = CEngine::GetInstance().GetNavMeshManager();

    std::string lNodeName;
    std::set< std::string > lNodeNamesFromXML;

    for (tinyxml2::XMLElement *iSceneNode = aElement->FirstChildElement(); iSceneNode != nullptr; iSceneNode = iSceneNode->NextSiblingElement())
    {
        CSceneNode* lNode = nullptr;
        lNodeName = iSceneNode->GetAttribute<std::string>("name", "");
        lNodeNamesFromXML.insert(lNodeName);

        if (strcmp(iSceneNode->Name(), "scene_mesh") == 0)
        {
            CSceneMesh* lTmpSceneMesh = (CSceneMesh*)(*this)(lNodeName);

            if (lTmpSceneMesh)
            {
                lTmpSceneMesh->Initialize(iSceneNode);
            }
            else
            {
                lNode = new CSceneMesh(iSceneNode);
                lNode->SetNodeType(CSceneNode::eMesh);
            }
        }
        else if (strcmp(iSceneNode->Name(), "scene_animated_model") == 0)
        {
            if (!Exist(lNodeName))
            {
                std::string l_CoreName = iSceneNode->GetAttribute<std::string>("core", "");
                int lGroup = iSceneNode->GetAttribute<int>("group", 0);
                CAnimatedCoreModel *l_AnimatedCoreModel = CEngine::GetInstance().GetAnimatedModelManager()(l_CoreName);
                if (l_AnimatedCoreModel != nullptr)
                {
                    lNode = new CSceneAnimatedModel(*iSceneNode);
                    ((CSceneAnimatedModel *)lNode)->Initialize(l_AnimatedCoreModel);
                    if (strcmp(lNodeName.c_str(), "player") == 0)
                    {
                        Quatf rotation = Quatf();
                        float height = iSceneNode->GetAttribute<float>("height", 1.1f);
                        float radius = iSceneNode->GetAttribute<float>("radius", 0.17f);
                        rotation.QuatFromYawPitchRoll(lNode->GetYaw(), lNode->GetPitch(), lNode->GetRoll());
                        CEngine::GetInstance().GetPhysXManager().AddCharacterController(lNodeName, height, radius, lNode->GetPosition(), rotation, "Default", 0.5f, lGroup);
                    }
                }
                lNode->SetNodeType(CSceneNode::eAnimatedModel);
            }
        }
        else if (strcmp(iSceneNode->Name(), "scene_basic_primitive") == 0)
        {
            lNode = new  CSceneBasicPrimitive(iSceneNode);
            lNode->SetNodeType(CSceneNode::eBasicPrimitive);
        }
        else if (strcmp(iSceneNode->Name(), "scene_light") == 0)
        {
            CLight *l_light = nullptr;
            CConstantBufferManager& lConstanBufferManager = CEngine::GetInstance().GetConstantBufferManager();
            lConstanBufferManager.mLightsDesc.m_LightEnabled[0] = 0;
            lConstanBufferManager.mLightsDesc.m_LightEnabled[1] = 0;
            lConstanBufferManager.mLightsDesc.m_LightEnabled[2] = 0;
            lConstanBufferManager.mLightsDesc.m_LightEnabled[3] = 0;
            if (lLM.Exist(lNodeName))
            {
                lNode = lLM(lNodeName);
                lLM.SetLightConstants(iSceneNode->GetAttribute<int>("id_light", 0), lLM(lNodeName));
                lNode->SetNodeType(CSceneNode::eLight);
            }
        }
        else if (strcmp(iSceneNode->Name(), "scene_particle") == 0)
        {
            lNode = new CParticleSystemInstance(iSceneNode);
            lNode->SetNodeType(CSceneNode::eParticle);
        }
        else if (strcmp(iSceneNode->Name(), "scene_navmesh") == 0)
        {
            lNode = l_NavMeshManager(lNodeName);
            lNode->SetNodeType(CSceneNode::eNavMesh);
        }
        else if (strcmp(iSceneNode->Name(), "scene_enemies") == 0)
        {
            CEnemy *l_enemy = nullptr;

            if (lEnemiesManager.Exist(lNodeName))
            {
                lNode = lEnemiesManager(lNodeName);
                lNode->SetNodeType(CSceneNode::eEnemy);
            }
        }

        if (lNode)
        {
            if (!Exist(lNode->GetName()))
            {
                lNode->SetParent(this);
                lOk &= Add(lNode->GetName(), lNode);
            }
        }
    }

    if (update)
    {
        std::set<std::string> lMissingNodes;

        for (std::vector<CSceneNode*>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); ++it)
        {
            if (lNodeNamesFromXML.find((*it)->GetName()) == lNodeNamesFromXML.end())
            {
                assert(lMissingNodes.count((*it)->GetName()) == 0);
                lMissingNodes.insert((*it)->GetName());
            }
        }

        for (std::set<std::string>::iterator iMissingNode = lMissingNodes.begin(); iMissingNode != lMissingNodes.end(); ++iMissingNode)
        {
            if ((*this)(*iMissingNode)->GetNodeType() == CSceneNode::eMesh)
            {
                ((CSceneMesh*)(*this)(*iMissingNode))->DeletePhysx();
            }

            Remove(*iMissingNode);
        }
    }

    return lOk;
}

void CLayer::DeleteAllNodes()
{
    std::set<std::string> lMissingNodes;

    for (std::vector<CSceneNode*>::iterator it = m_ResourcesVector.begin(); it != m_ResourcesVector.end(); ++it)
    {
        assert(lMissingNodes.count((*it)->GetName()) == 0);
        lMissingNodes.insert((*it)->GetName());
    }

    for (std::set<std::string>::iterator iMissingNode = lMissingNodes.begin(); iMissingNode != lMissingNodes.end(); ++iMissingNode)
    {
        if ((*this)(*iMissingNode)->GetNodeType() == CSceneNode::eMesh)
        {
            ((CSceneMesh*)(*this)(*iMissingNode))->DeletePhysx();
        }

        Remove(*iMissingNode);
    }
}

bool CLayer::Update(float elapsedTime)
{
    bool lOk = true;
    for (std::vector<CSceneNode*>::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        lOk &= (*iSceneNode)->Update(elapsedTime);
    }
    return lOk;
}

bool CLayer::Render()
{
    bool lOk = true;
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();

    for (TVectorResources::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        if ((*iSceneNode)->GetNodeType() != CSceneNode::eLight)
        {
            lOk &= (*iSceneNode)->Render(lRenderManager);
        }
    }

    return lOk;
}

bool CLayer::Refresh()
{
    CLightManager &lLM = CEngine::GetInstance().GetLightManager();
    std::set<std::string> lMissingNodes;

    for (TVectorResources::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
    {
        if ((*iSceneNode)->GetNodeType() == CSceneNode::eLight)
        {
            if (!lLM.Exist((*iSceneNode)->GetName()))
            {
                lMissingNodes.insert((*iSceneNode)->GetName());
            }
        }
    }

    for (std::set<std::string>::iterator iMissingNode = lMissingNodes.begin(); iMissingNode != lMissingNodes.end(); ++iMissingNode)
    {
        Remove(*iMissingNode);
    }

    return false;
}

std::vector<CSceneNode*> CLayer::GetNodes()
{
    return m_ResourcesVector;
}

void CLayer::DrawImgui()
{
    ImGui::Checkbox("Active", &m_Active);
    if (m_Active)
    {
        ImGui::BeginChild("#layer", ImVec2(800, 400), true, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::PushItemWidth(-130);

        for (std::vector<CSceneNode*>::iterator iSceneNode = m_ResourcesVector.begin(); iSceneNode != m_ResourcesVector.end(); ++iSceneNode)
        {
            ImGui::PushID((*iSceneNode)->GetName().c_str());
            switch ((*iSceneNode)->GetNodeType())
            {

            case CSceneMesh::eMesh:
            {
                (*iSceneNode)->DrawImgui();
            }
            break;

            case CSceneNode::eAnimatedModel:
            {
                // TODO animated core models deben formar parte de la escena
                ((CSceneAnimatedModel *)(*iSceneNode))->DrawImgui();
                CAnimatedCoreModel *lAnimatedCoreModel = CEngine::GetInstance().GetAnimatedModelManager()((*iSceneNode)->GetName());

                if (lAnimatedCoreModel != nullptr)
                    lAnimatedCoreModel->DrawImgui();
            }
            break;

            case CSceneAnimatedModel::eBasicPrimitive:
            {
                ((CSceneBasicPrimitive *)(*iSceneNode))->DrawImgui();
            }
            break;

            case CLight::eLight:
            {
                (*iSceneNode)->DrawImgui();
            }
            break;

            case CSceneNode::eParticle:
            {
                CParticleSystemType *lParticle = CEngine::GetInstance().GetParticleManager()((*iSceneNode)->GetName());
                if (lParticle != nullptr)
                {
                    lParticle->DrawImgui();
                }
            }
            break;
            case CSceneNode::eNavMesh:
            {
                CNavMesh *lNavMesh = CEngine::GetInstance().GetNavMeshManager()((*iSceneNode)->GetName());
                if (lNavMesh != nullptr)
                    lNavMesh->DrawImgui();
            }
            /* case CSceneNode::eLaser:
             {
                 CLaser *lLaser = CEngine::GetInstance().GetLaserManager()((*iSceneNode)->GetName());
                 if (lLaser != nullptr)
                     lLaser->DrawImgui();
             }
             break;*/
            default:
            {
                LOG_WARNING_APPLICATION("Unknown Scene Node Type");
            }
            break;
            }

            ImGui::PopID();
        }
    }
}

CSceneNode* CLayer::GetSceneNode(std::string aName)
{
    CSceneNode* lOut = nullptr;

    auto lNodePair = m_ResourcesMap.find(aName);

    if (lNodePair != m_ResourcesMap.end())
    {
        lOut = lNodePair->second.m_Value;
    }

    return lOut;
}
