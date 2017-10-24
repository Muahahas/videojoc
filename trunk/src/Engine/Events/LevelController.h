#pragma once

#ifndef _LevelController_
#define _LevelController_
#include "Utils/Defines.h"

#include "Engine/Engine.h"
#include "Input/ActionManager.h"
#include "Graphics/Scenes/SceneManager.h"


class CLevelController
{

public:

    CLevelController();
    CLevelController(int lvl);
    ~CLevelController();
    void Init();
    void PlayerDetected(std::string detectorName );  //Nombre del dron/guardia

    void Update(float elapsedTime);
    void PauseGame();
    void ResumeGame();
    GET_SET(bool,TimePaused)

private:
    int Level = 1;
    CEngine* m_Engine;
    CSceneManager* m_SceneManager;
    CPhysXManager* m_PhysxManager;
    CActionManager* m_ActionManager;
    bool m_TimePaused;


};





#endif