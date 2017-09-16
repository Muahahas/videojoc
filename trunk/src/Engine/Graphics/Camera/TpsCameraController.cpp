#include "TpsCameraController.h"
#include "Engine/Engine.h"
#include "Physx/PhysxManager.h"
#include "Utils/Logger.h"

void CTpsCameraController::Update(float ElapsedTime)
{

    center = m_player->m_Position+playerHeight;
    yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    zoomSpeed = 0.5f*(*actionManager)("zoom")->value;

    yaw += yawSpeed * ElapsedTime;
    pitch += pitchSpeed * ElapsedTime;
    roll += rollSpeed * ElapsedTime;
    zoom += zoomSpeed * ElapsedTime;

    if (pitch > maxPitch)
        pitch = maxPitch;
    if (pitch < minPitch)
        pitch = minPitch;

    if (zoom > maxZoom)
        zoom = maxZoom;
    if (zoom < minZoom)
        zoom = minZoom;

    if (yaw > mathUtils::PiTimes<float>())
        yaw -= mathUtils::PiTimes<float>(2.0f);
    if (yaw < -mathUtils::PiTimes<float>())
        yaw += mathUtils::PiTimes<float>(2.0f);

    if (roll > mathUtils::PiTimes<float>())
        roll -= mathUtils::PiTimes<float>(2.0f);
    if (roll < -mathUtils::PiTimes<float>())
        roll += mathUtils::PiTimes<float>(2.0f);

    // set CameraControllerData
    m_Front.x = sin(yaw) * cos(-pitch);
    m_Front.y = -sin(-pitch);
    m_Front.z = -cos(yaw) * cos(-pitch);



    m_Position = center - m_Front * zoom;
    //Vect3f dir = m_Position - m_player->m_Position;
    Vect3f origin = m_player->m_Position + (0.25f * -m_Front);
    CPhysXManager::RaycastData *data = new CPhysXManager::RaycastData;
    bool hit = physX->Raycast(origin, m_Position, 0,data);
    if (hit)
    {
        LOG_INFO_APPLICATION("HIT \\(^-^)/");
        data->distance;
        m_Position = center - m_Front * data->distance;
    }
    else
    {
        LOG_INFO_APPLICATION("NOt HIT \\(^-^)/");
    }



}

void CTpsCameraController::Init(CCharacterController* player)
{
    m_player = player;
    actionManager = &CEngine::GetInstance().GetActionManager();
    physX = &CEngine::GetInstance().GetPhysXManager();
}
