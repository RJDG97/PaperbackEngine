/**********************************************************************************
*\file         Camera.cpp
*\brief        Contains definition of functions and variables used for
*			   the Camera System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/CameraSystem.h"
#include "Engine/Core.h"
#include "Manager/EntityManager.h"

void CameraSystem::Init()
{
    component_manager_ = &*CORE->GetManager<ComponentManager>();
    camera_arr_ = component_manager_->GetComponentArray<Camera>();
    windows_system_ = &*CORE->GetSystem<WindowsSystem>();

    win_size_ = {static_cast<float>(windows_system_->GetWinWidth()), static_cast<float>(windows_system_->GetWinHeight())};
}

void CameraSystem::Update(float frametime)
{
    (void)frametime;

    for (CameraIt camera = camera_arr_->begin(); camera != camera_arr_->end(); ++camera) {

        CameraUpdate(camera->second);
    }
}

void CameraSystem::Draw()
{
}

void CameraSystem::AddCameraComponent(EntityID id, Camera* camera)
{
    M_DEBUG->WriteDebugMessage("Adding Text Renderer Component to entity: "
        + std::to_string(id) + "\n");

    camera_arr_->AddComponent(id, camera);
}

void CameraSystem::RemoveCameraComponent(EntityID id)
{
    camera_arr_->RemoveComponent(id);
}

std::string CameraSystem::GetName()
{
    return "CameraSystem";
}

void CameraSystem::SendMessageD(Message* m)
{
    if (camera_arr_->size() == 0)
    {
        return;
    }

    switch (m->message_id_) {

        case MessageIDTypes::CAM_UPDATE_POS: {

            MessagePhysics_Motion* msg = dynamic_cast<MessagePhysics_Motion*>(m);
            CameraMove(GetMainCamera(), msg->new_vec_);
            break;
        }

        /*
        case MessageIDTypes::CHANGE_ANIMATION_1: {

            //Temporary
            if (CORE->GetManager<EntityManager>()->GetPlayerEntities().size() > 0)
            {
                SetTarget(GetMainCamera(), CORE->GetManager<EntityManager>()->GetPlayerEntities()[0]);
                ToggleTargeted(GetMainCamera());
            }

            break;
        }*/

        case MessageIDTypes::CAM_ZOOM_IN: {

            Camera* camera = GetMainCamera();
            CameraZoom(camera, camera->cam_zoom_ + 0.1f);
            break;
        }

        case MessageIDTypes::CAM_ZOOM_OUT: {

            Camera* camera = GetMainCamera();
            CameraZoom(camera, camera->cam_zoom_ - 0.1f);
            break;
        }
    }
}

void CameraSystem::CameraUpdate(Camera* camera)
{
    const float global_scale = CORE->GetGlobalScale();
    Vector2D position = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID())->GetPosition() * global_scale;

    glm::mat3 view_xform { 1 , 0 , 0,
                           0 , 1 , 0,
                           -position.x , -position.y , 1 };

    // compute other matrices ...
    glm::mat3 camwin_to_ndc_xform { 2 / camera->cam_size_.x , 0 , 0,
                                    0 , 2 / camera->cam_size_.y , 0,
                                    0 , 0 , 1 };

    camera->world_to_ndc_xform_ = camwin_to_ndc_xform * view_xform;
}

void CameraSystem::CameraZoom(Camera* camera, float zoom)
{
    if (zoom < 0.05f)
    {
        return;
    }

    camera->cam_zoom_ = zoom;

    camera->cam_size_ = glm::vec2{ windows_system_->GetWinWidth(),
                           windows_system_->GetWinHeight() } / camera->cam_zoom_;
}

void CameraSystem::CameraMove(Camera* camera, Vector2D displacement)
{
    Transform* transform = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID());
    Vector2D position = transform->GetPosition();
    transform->AddOffset({ displacement.x * 0.01f, displacement.y * 0.01f });

    //transform->SetPosition({ position.x + displacement.x * 0.05f, position.y + displacement.y * 0.05f });
}

void CameraSystem::CameraSetPosition(Camera* camera, Vector2D position)
{
    Transform* transform = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID());
    transform->SetPosition({ position.x, position.y });
}

Camera* CameraSystem::GetMainCamera()
{
    if (camera_arr_->size() == 0)
    {
        return nullptr;
    }

    return camera_arr_->begin()->second;
}

Vector2D CameraSystem::GetMainCameraPos()
{
    if (camera_arr_->size() == 0)
    {
        return {};
    }

    Camera* cam = GetMainCamera();
    return CORE->GetManager<ComponentManager>()->
            GetComponent<Transform>(cam->GetOwner()->GetID())->GetPosition();
}

Vector2D CameraSystem::UIToGameCoords(const Vector2D& ui_pos) {

    Camera* cam = GetMainCamera();

    if (cam) {

        float global_scale = CORE->GetGlobalScale();
        Vector2D cam_pos = GetMainCameraPos();
        return ((ui_pos - 0.5f * win_size_) / *cam->GetCameraZoom() - cam_pos) / global_scale;
    }

    return {};
}

Vector2D CameraSystem::GameCoordsToUI(const Vector2D& go_pos) {

    Camera* cam = GetMainCamera();

    if (cam) {

        float global_scale = CORE->GetGlobalScale();
        Vector2D cam_pos = -GetMainCameraPos() * global_scale;
        Vector2D translation{ cam_pos * *cam->GetCameraZoom() + 0.5f * win_size_ };
        return (go_pos * global_scale * *cam->GetCameraZoom() + Vector2D{ translation.x, translation.y });
    }

    return {};
}