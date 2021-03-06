#include "FpsCameraController.h"
#include "Input/ActionManager.h"
#include "Engine/Engine.h"

void CFpsCameraController::Update(float ElapsedTime)
{
    CCameraController::Update(ElapsedTime);
    CActionManager* actionManager = &CEngine::GetInstance().GetActionManager();
    center = CEngine::GetInstance().GetRenderManager().m_SphereOffset;
    xSpeed = 0.1f * (*actionManager)("x_move")->value;
    zSpeed = 0.1f * (*actionManager)("z_move")->value;

    yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    yaw += yawSpeed * ElapsedTime;
    pitch += pitchSpeed * ElapsedTime;
    roll += rollSpeed * ElapsedTime;

    if (pitch > maxPitch)
        pitch = maxPitch;
    if (pitch < minPitch)
        pitch = minPitch;

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

    // TODO roll

    m_Position += zSpeed * m_Front;

    Vect3f right = m_Front ^ m_Up;
    m_Position += xSpeed * right;

    m_Position.y = 1;
    /*
    m_Position.x = clamp(m_Position.x, maxDistance, -maxDistance);
    m_Position.z = clamp(m_Position.z, maxDistance, -maxDistance);
    */
}
