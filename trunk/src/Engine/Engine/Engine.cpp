#include "Engine.h"
#include "Input\ActionManager.h"
#include "imgui_impl_dx11.h"
#include "ImGUI\imgui.h"
#include "Camera\CameraController.h"
#include "Textures\TextureManager.h"
#include "Mesh\Mesh.h"
#include "Mesh\MeshManager.h"
#include "RenderPipeline\RenderPipeline.h"

#undef BUILD_GET_SET_ENGINE_MANAGER

CEngine::CEngine()

    : m_RenderManager(nullptr)
    , m_ActionManager(nullptr)
    , m_CameraController(nullptr)
    , m_Clock()
    , m_PrevTime(m_Clock.now())
{
    deltaTime = 0;
    cameraSelector = 0;
    l_prevCameraSelector = 0;
}

void CEngine::Init(HWND hWnd)
{
    ImGui_ImplDX11_Init(hWnd, m_RenderManager->GetDevice(), m_RenderManager->GetDeviceContext());
}

void CEngine::ProcessInputs()
{
    ImGui_ImplDX11_NewFrame();
    m_ActionManager->Update();
}

void CEngine::Update()
{

    auto currentTime = m_Clock.now();
    std::chrono::duration<float> chronoDeltaTime = currentTime - m_PrevTime;
    m_PrevTime = currentTime;

    float dt = chronoDeltaTime.count() > 0.5f ? 0.5f : chronoDeltaTime.count();
    deltaTime = dt;

    // Reiniciem posici� de l'esfera quan canviem de camera
    if (cameraSelector != l_prevCameraSelector)
    {
        m_RenderManager->m_SphereOffset = Vect3f(0, 0, 0);
    }

    l_prevCameraSelector = cameraSelector;

    switch (cameraSelector)
    {
    case 0: //Orbital
        SetCameraController(&orbitalCam);
        orbitalCameraUpdate(*m_CameraController, m_ActionManager, dt);

        break;
    case 1: //FPS
        SetCameraController(&fpsCam);
        fpsCameraUpdate( *m_CameraController,m_ActionManager, dt);

        break;
    case 2: //TPS
        SetCameraController(&tpsCam);
        sphereUpdate(*m_RenderManager, m_ActionManager, m_CameraController->getFront(), m_CameraController->getUp());
        tpsCameraUpdate(*m_CameraController, m_ActionManager, m_RenderManager->m_SphereOffset, dt);

        break;
    default:
        break;
    }

    m_CameraController->Update(dt);

    m_CameraController->SetToRenderManager(*m_RenderManager);
}

void CEngine::Render()
{
    //m_RenderPipeline->Execute();


    m_RenderManager->BeginRender();

    static bool show_app_auto_resize = false;
    ImGui::Begin("Motor3D", &show_app_auto_resize, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Frame rate: ");
    ImGui::SameLine();
    ImGui::Text("FPS", 1/deltaTime, 0.0f, -1, 0);
    // ImGui::ShowTestWindow();

    ImGui::RadioButton("Orbital", &cameraSelector, 0);
    ImGui::SameLine();
    ImGui::RadioButton("FPS", &cameraSelector, 1);
    ImGui::SameLine();
    ImGui::RadioButton("TPS", &cameraSelector, 2);

    //m_TextureManager->GetTexture("data/textures/Box001LightingMap.tga");
    //ImTextureID tex_id = m_TextureManager->GetTexture();
    //ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

    /*    CMesh* lMesh = m_MeshManager->GetMesh("data/meshes/Plane001.mesh");
        lMesh->Render(*m_RenderManager);
    	*/


    m_RenderManager->DrawGrid(1, 1, 1, CColor(0, 0, 0, 1));
    switch (cameraSelector)
    {
    case 0: //Orbital
        m_RenderManager->DrawSphere(1, CColor(1, 1, 0, 1));

        break;
    case 1: //FPS
        m_RenderManager->m_SphereOffset = Vect3f(0, 3, 0);
        m_RenderManager->DrawSphere(1, CColor(0, 1, 1, 1));

        break;
    case 2: //TPS
        sphereRender(*m_RenderManager);
        break;
    default:
        break;
    }
    // -----------------------------

    // game render

    // -----------------------------

    if (ImGui::CollapsingHeader("Lights"))
    {
        //crear un render imgui del light manager
    }

    ImGui::End();
    ImGui::Render();

    m_RenderManager->EndRender();
}

void CEngine::fpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt)
{

    /*ImGui::Text("Mou la camera amb WASD.");
    ImGui::Text("Orienta't amb el ratoli.");
    ImGui::Text("Vigila el cap.");*/
    CFpsCameraController *fpsCamera = static_cast<CFpsCameraController*>(&camera);
    fpsCamera->xSpeed = 0.1f * (*actionManager)("x_move")->value;
    fpsCamera->zSpeed = 0.1f * (*actionManager)("z_move")->value;

    fpsCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    fpsCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    fpsCamera->Update(dt);
}

void CEngine::orbitalCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt)
{

    /*ImGui::Text("Gira la roda del mouse per controlar el zoom.");
    ImGui::Text("Apreta qualsevol boto del ratoli per habilitar la rotacio.");*/
    CSphericalCameraController *sphericalCamera = static_cast<CSphericalCameraController*>(&camera);
    sphericalCamera->zoomSpeed = (*actionManager)("zoom")->value;

    if ((*actionManager)("enable_rotation")->active)
    {
        sphericalCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
        sphericalCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;
    }
    else
    {
        sphericalCamera->yawSpeed = 0.0;
        sphericalCamera->pitchSpeed = 0.0;
    }

    sphericalCamera->Update(dt);
}

void CEngine::tpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, Vect3f sphereCenter, float dt)
{

    /*ImGui::Text("Mou l'esfera amb WASD");
    ImGui::Text("Orienta't amb el ratoli.");
    ImGui::Text("Amb la roda del ratoli pots controlar el zoom.");*/
    CTpsCameraController *tpsCamera = static_cast<CTpsCameraController*>(&camera);
    tpsCamera->center = sphereCenter;

    tpsCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    tpsCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    tpsCamera->zoomSpeed = (*actionManager)("zoom")->value;

    tpsCamera->Update(dt);
}


float clamp(float x, float upper, float lower)
{
    return min(upper, max(x, lower));
}
void CEngine::sphereUpdate(CRenderManager& renderManager, CActionManager* actionManager, Vect3f front, Vect3f up)
{

    Vect3f right = front ^ up;
    float smoother = 0.1f;

    renderManager.m_SphereOffset += (*actionManager)("z_move")->value * front * smoother;
    renderManager.m_SphereOffset += (*actionManager)("x_move")->value * right * smoother;

    renderManager.m_SphereOffset.x = clamp(renderManager.m_SphereOffset.x, 5.0, -5.0);
    renderManager.m_SphereOffset.y = 1;
    renderManager.m_SphereOffset.z = clamp(renderManager.m_SphereOffset.z, 5.0, -5.0);

}

void CEngine::sphereRender(CRenderManager& renderManager)
{

    renderManager.DrawSphere(1, CColor(1, 1, 1, 1));
}