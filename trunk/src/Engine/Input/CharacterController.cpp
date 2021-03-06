#include "CharacterController.h"
#include "Engine/Engine.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/IA/EnemiesManager.h"
#include "Physx/PhysxManager.h"
#include "Utils/CheckedRelease.h"
#include "Events/LevelController.h"
#include "Graphics/IA/EnemyAnimated.h"
#include "Graphics/IA/Guard.h"

CCharacterController::~CCharacterController()
{
    base::utils::CheckedDelete(sceneManager);
    base::utils::CheckedDelete(actionManager);
    base::utils::CheckedDelete(physXManager);
}

void CCharacterController::Update(float ElapsedTime)
{
    CLevelController *contr = CEngine::GetInstance().m_LevelController;
    if (player != nullptr && !contr->GetTimePaused() && m_KILLING)
    {
        killingTimer += ElapsedTime;
        //player->ClearActiveAnimationCycle(0.5f);
        //player->BlendCycle(5, 1, 0.5f);


        if (killingTimer>2.95f)//2a 3.45f
        {
            m_KILLING = false;
        }

    }
    else if (player != nullptr && !contr->GetTimePaused())
    {
        CCameraController* cam = &CEngine::GetInstance().GetCameraManager().GetCurrentCamera();
        Vect3f l_Front = cam->GetFront();
        Vect3f l_Up = cam->GetUp();
        Vect3f l_Right = l_Front ^ l_Up;

        l_Front.y = 0;
        l_Right.y = 0;
        float x = (*actionManager)("x_move")->value;
        float z = (*actionManager)("z_move")->value;
        float run = (*actionManager)("run")->value;
        float crouch = (*actionManager)("crouch")->value;
        float kill = (*actionManager)("mouse_left_pressed")->value;
        float l_Speed = m_Speed;
        if (kill > 0.1)
        {

            CPhysXManager::RaycastData *data = new CPhysXManager::RaycastData;

            Vect3f dist = l_Front * killDistance*1.5;
            Vect3f lKPos = m_Position + Vect3f(0, 1, 0) + (l_Front*0.5f);
            bool hit = physXManager->RaycastCam(lKPos, lKPos + dist, 0, data);
            if (hit)
            {
                std::string actorName = data->actor;
                if (actorName.find("Guardia")==0)
                {
                    CSceneManager* sceneMan = &CEngine::GetInstance().GetSceneManager();
                    Vect3f enemForward = sceneMan->GetCurrentScene()->GetLayer("guardias")->GetSceneNode(actorName)->GetForward();

                    CGuard* enemy = (CGuard*)(sceneMan->GetCurrentScene()->GetLayer("guardias")->GetSceneNode(actorName));
                    enemy->dieEnemy(data->position);
                    player->ClearActiveAnimationCycle(0.5f);
                    player->BlendCycle(5, 1, 0.5f);
                    player->SetForward(dist.GetNormalized());
                    m_KILLING = true;
                    killingTimer = 0;
                }

            }


        }
        m_CrouchingBOOL = false;
        m_RunningBOOL = false;
        if (run > 0.f && z > 0.1f)
        {
            x /= 2;
            l_Speed *= 1.4;
            m_RunningBOOL = true;
        }

        if (crouch > 0.1f)
        {

            m_CrouchingBOOL = true;
            m_RunningBOOL = false;
            l_Speed *= .6f;
        }

        if (!m_KILLING)
        {
            Vect3f forwardMove = z * l_Front;
            Vect3f horizontalMove = x * l_Right;
            // m_Movement = { x, 0.0f, z };
            m_Movement = forwardMove + horizontalMove;
            if (m_Movement != (0, 0, 0))
            {
                if (crouch > 0.1f)
                {
                    player->ClearActiveAnimationCycle(0.5f);
                    player->BlendCycle(4, 1, 0.5f);
                    //player->BlendCycle(1, 1, 0); crouch moving animation
                }
                else if (run > 0.1f)
                {
                    player->ClearActiveAnimationCycle(0.5f);
                    player->BlendCycle(3, 1, 0.5f);
                    //player->BlendCycle(4, 1, 0); run animation
                }
                else
                {

                    player->ClearActiveAnimationCycle(0.5f);
                    player->BlendCycle(2, 1, 0.5f);
                }


                m_Movement = m_Movement.GetNormalized();
                Vect3f l_Dir = m_Movement;
                Vect3f l_For = player->GetForward();
                l_For.Lerp(l_Dir, m_RotationSPeed);
                m_Movement *= l_Speed * ElapsedTime;
                player->SetForward(l_Dir);

            }
            else
            {
                if (crouch > 0.1f)
                {
                    player->ClearActiveAnimationCycle(0.5f);
                    player->BlendCycle(1, 1, 0.5f);
                }
                else
                {
                    player->ClearActiveAnimationCycle(0.5f);
                    player->BlendCycle(0, 1, 0.5f);
                }
            }
            physXManager->MoveCharacterController("player", m_Movement, PHYSX_UPDATE_STEP);

            //player->SetPosition(m_Position);
            m_Position = physXManager->GetActorPosition("player");

            player->SetPosition(m_Position);



        }
    }
    else if (player && contr->GetTimePaused())
    {
        player->ClearActiveAnimationCycle(0.5f);
        player->BlendCycle(0, 1.f, 0.5);
    }
}




void CCharacterController::Init(CSceneManager* sceneManager)
{
    actionManager = &CEngine::GetInstance().GetActionManager();
    physXManager = &CEngine::GetInstance().GetPhysXManager();
    sceneManager = &CEngine::GetInstance().GetSceneManager();
    m_CrouchingBOOL = false;
    m_RunningBOOL = false;

    if (sceneManager->GetCurrentScene()->Exist("LPlayer"))
    {
        CLayer* layer = sceneManager->GetCurrentScene()->GetLayer("LPlayer");
        if (layer->Exist("player"))
        {
            CSceneAnimatedModel* anim = (CSceneAnimatedModel*)(*layer)("player");
            player = anim;
        }
    }

    if (player != nullptr)
        //    player->SetPosition(m_Position);
        m_Position = player->GetPosition();


}


Vect3f CCharacterController::getFront()
{

    if (player != nullptr)
    {
        return player->GetForward();
    }
    else
    {
        return Vect3f(0, 0, 0);
    }
}

void CCharacterController::moveToLastCheckpoint(Vect3f lastCheckpoint, Quatf rotation)
{
    if (player != nullptr)
    {
        PxController* contr = physXManager->GetCharacterController("player");
        contr->setPosition(PxExtendedVec3(lastCheckpoint.x, lastCheckpoint.y, lastCheckpoint.z));

        player->SetPosition(lastCheckpoint);
        player->SetYaw(rotation.GetYaw());
        player->SetPitch(rotation.GetPitch());
        player->SetRoll(rotation.GetRoll());
        m_Position = lastCheckpoint;

    }

}


