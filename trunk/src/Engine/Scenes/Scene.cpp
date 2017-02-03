#include "Scene.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "XML/XML.h"

CScene::CScene(const std::string& aName)
    : CName(aName)
{}

CScene::~CScene() {}

bool CScene::Load(const std::string& aFilename)
{
    bool lOk = true;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lScene = document.FirstChildElement("scene");

        if (lScene)
        {
            for (tinyxml2::XMLElement *iLayer = lScene->FirstChildElement(); iLayer != nullptr; iLayer = iLayer->NextSiblingElement())
            {
                if (strcmp(iLayer->Name(), "layer") == 0)
                {
                    CLayer* lLayer = new CLayer(iLayer->GetAttribute<std::string>("name", ""));
                    lLayer->Load(iLayer);
                    lOk &= Add(lLayer->GetName(), lLayer);
                }
            }
        }
    }

    return lOk;
}

bool CScene::Update(float elapsedTime)
{
    // #ALEX: las layers tienen booleano para mantenerlas activas?
    bool lOk = true;

    for (TMapResources::iterator iLayer = m_ResourcesMap.begin(); iLayer != m_ResourcesMap.end(); ++iLayer)
    {
        lOk &= iLayer->second.m_Value->Update(elapsedTime);
    }

    return lOk;
}

bool CScene::Render()
{
    // #ALEX: este render debe pintar todas las layers?
    bool lOk = true;

    for (TMapResources::iterator iLayer = m_ResourcesMap.begin(); iLayer != m_ResourcesMap.end(); ++iLayer)
    {
        lOk &= iLayer->second.m_Value->Render();
    }

    return lOk;
}

bool CScene::Render(const std::string& aLayerName)
{
    bool lOk = true;

    lOk &= m_ResourcesMap.find(aLayerName)->second.m_Value->Render();

    return lOk;
}