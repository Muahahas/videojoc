#pragma once
#ifndef _ENGINE_CHARACTERCONTROLLER_20170310_H
#define _ENGINE_CHARACTERCONTROLLER_20170310_H

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif
#include "Math/Vector3.h"
#include "ActionManager.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Animation/SceneAnimatedModel.h"

class CPhysXManager;
class CCharacterController
{

public:
    CCharacterController() :
        m_Movement(0, 0, 0),
        m_Position(0, 0, 0),
        m_Speed(2.f),
        m_RotationSPeed(0.2f),
        player(nullptr),
        killDistance(2.f),
        m_KILLING(false)
    {
    }

    virtual ~CCharacterController();

    virtual void Update(float ElapsedTime);
    virtual void Init(CSceneManager* sceneManager);
    Vect3f m_Movement;
    Vect3f m_Position;
    float m_Speed;
    float m_RotationSPeed;
    CSceneAnimatedModel* player;
    Vect3f getFront();
    void moveToLastCheckpoint(Vect3f lastCheckpoint,Quatf rotation);
    bool m_CrouchingBOOL;
    bool m_RunningBOOL;


private:
    Vect3f m_Gravity = (0.0f, 9.81f, 0.0f);
    CActionManager* actionManager;
    CPhysXManager* physXManager;
    CSceneManager* sceneManager;
    float killDistance;
    bool m_KILLING;
    Vect3f killingPosition;
    Vect3f enemyPos;
    float killingTimer = 0.f;
};

#endif //_ENGINE_CAMERACONTROLLER_CPB_20161126_H