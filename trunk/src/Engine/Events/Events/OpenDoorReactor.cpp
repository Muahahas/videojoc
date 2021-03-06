#include "OpenDoorReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "Graphics/Scenes/SceneMesh.h"
#include "Physx/PhysxManager.h"

#include "Utils/CheckedRelease.h"

COpenDoorReactor::COpenDoorReactor() {}

COpenDoorReactor::~COpenDoorReactor()
{
    __H_CHECKED_RELEASE__(mDoor);
}

void COpenDoorReactor::Load(CXMLElement* aElement)
{
    std::string lLayerName = aElement->GetAttribute<std::string>("layer", "");
    std::string lObjectName = aElement->GetAttribute<std::string>("obj_name", "");

    assert(lLayerName != "");
    assert(lObjectName != "");

    CLayer* lLayer = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName);
    if (lLayer && lLayer->GetActive())
    {
        mDoor = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName)->GetSceneNode(lObjectName);
    }
    else
    {
        mDoor = false;
    }
}

void COpenDoorReactor::React()
{
    //LOG_INFO_APPLICATION("Hold the DOOOOOOOR! HOLD THE DOOOOOR!");
    if (mDoor)
    {
        ((CSceneMesh*)mDoor)->Deactivate();
    }
    m_Finished = true;
}

void COpenDoorReactor::Update(float elapsedTime)
{
    //LOG_INFO_APPLICATION("React updating!");

    if (!mDoor)
    {
        // LOG_INFO_APPLICATION("Door is missing in scene!");
        return;
    }

    if (mDoor->GetActive())
    {
        /*
        Esto no va fino.
        Hace falta mover primero el physx, y luego actualizar la malla con esta posicion.
        El problema es que physx pivota sobre el centro, por lo tanto hay dos opciones:
        - Hacer un joint
        - Hacer el movimiento m�s trapero

        Se podr�a mejorar tambi�n poniendo un objeto din�mico y kinematico al collider de la puerta... pero se desfasan los colliders respecto la malla.
        La solucion chapucera de las mallas est�ticas no parece funcionar igual de bien.

        mDoor->SetYaw(mDoor->GetYaw() + elapsedTime);

        Quatf rotation = Quatf();
        rotation.QuatFromYawPitchRoll(mDoor->GetYaw(), mDoor->GetPitch(), mDoor->GetRoll());

        Vect3f position = mDoor->GetPosition4Physx();

        CEngine::GetInstance().GetPhysXManager().SetActorTransform(mDoor->GetName(), position, rotation);
        */
    }
}
