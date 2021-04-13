/**********************************************************************************
*\file         Camera.cpp
*\brief        Contains definition of functions and variables used for
*			   the Camera System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/CameraSystem.h"
#include "Engine/Core.h"
#include "Manager/EntityManager.h"
#include "Systems/Game.h"

CameraSystem::Shake::Shake(float duration, float amplitude, float delay) :
    duration_ { duration },
    elapsed_time_ { 0.0f },
    amplitude_ { amplitude },
    delay_ {delay}
{

}

void CameraSystem::Init()
{
    component_manager_ = &*CORE->GetManager<ComponentManager>();
    camera_arr_ = component_manager_->GetComponentArray<Camera>();
    windows_system_ = &*CORE->GetSystem<WindowsSystem>();

    win_size_ = {static_cast<float>(windows_system_->GetWinWidth()), static_cast<float>(windows_system_->GetWinHeight())};
    shake_angle_timer = 0.0f;
    total_magnitude = 0.0f;
}

void CameraSystem::Update(float frametime)
{
    for (CameraIt camera = camera_arr_->begin(); camera != camera_arr_->end(); ++camera) {

        CameraUpdate(camera->second);
    }

    if (shakes_.size() > 0)
    {
        for (auto it = shakes_.begin(); it != shakes_.end(); )
        {
            if (it->delay_ > 0.0f)
            {
                it->delay_ -= frametime;
                ++it;
                continue;
            }

            it->elapsed_time_ += frametime;
            it->amplitude_ *= (1.0f - 0.5f * it->elapsed_time_ / it->duration_);
            total_magnitude = it->amplitude_;

            if (it->elapsed_time_ >= it->duration_)
            {
                it = shakes_.erase(it);
            }

            else
            {
                ++it;
            }
        }

        shake_angle_timer -= frametime;

        if (shake_angle_timer <= 0.0f)
        {
            shake_angle += 180.0f + distribution(generator);
            shake_offset = { sinf(shake_angle), cosf(shake_angle)};
            shake_angle_timer = max(0.05f, 0.1f * total_magnitude * total_magnitude);
        }
    }

    else
    {
        total_magnitude = 0.0f;
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

        case MessageIDTypes::CHANGE_ANIMATION_1: {

            break;
        }

        case MessageIDTypes::CAM_ZOOM_IN: {

            if (CORE->GetSystem<Game>()->GetStateName() == "Editor")
            {
                Camera* camera = GetMainCamera();
                CameraZoom(camera, camera->cam_zoom_ + 0.1f);
            }

            break;
        }

        case MessageIDTypes::CAM_ZOOM_OUT: {

            if (CORE->GetSystem<Game>()->GetStateName() == "Editor")
            {
                Camera* camera = GetMainCamera();
                CameraZoom(camera, camera->cam_zoom_ - 0.1f);
            }

            break;
        }
    }
}

void CameraSystem::CameraUpdate(Camera* camera)
{
    const float global_scale = CORE->GetGlobalScale();
    Vector2D position = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID())->GetPosition();

    if (camera->target_)
    {
        Vector2D target_position = *camera->target_;
        Vector2D move_dir = (target_position - position) * camera->speed_;
        component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID())->SetPosition(
                                                position + move_dir + shake_offset * total_magnitude);
    }

    else if (CORE->GetSystem<Game>()->GetStateName() != "Editor")
    {
        Vector2D move_dir = Vector2D{0.0f, 0.0f} - position * camera->speed_;
        component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID())->SetPosition(
                                                position + move_dir + shake_offset * total_magnitude);
    }

    position *= global_scale;

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
    Vector2D scaled_displacement = PE_FrameRate.GetFixedDelta() * displacement;
    Transform* transform = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID());
    Vector2D position = transform->GetPosition();
    transform->SetPosition({ position.x + scaled_displacement.x, position.y + scaled_displacement.y});
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

void CameraSystem::ScreenShake(float duration, float magnitude, float delay) {

    shakes_.push_back({ duration, magnitude, delay });

    if (shakes_.size() == 0)
    {
        shake_angle_timer = 0.0f;
    }
}

void CameraSystem::TargetPlayer()
{
    Entity* player = CORE->GetManager<EntityManager>()->GetPlayerEntities();

    if (player)
    {
        GetMainCamera()->target_ = component_manager_->GetComponent<Transform>(player->GetID())->GetPositionPtr();
    }
}

void CameraSystem::TargetVector(Vector2D* target)
{
    GetMainCamera()->target_ = target;
}