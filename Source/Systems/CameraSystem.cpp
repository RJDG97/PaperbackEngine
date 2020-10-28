#include "Systems\CameraSystem.h"
#include "Engine/Core.h"

void CameraSystem::Init()
{
    windows_system_ = &*CORE->GetSystem<WindowsSystem>();

    cam_pos_ = glm::vec2{ 0, 0 };
    cam_size_ = glm::vec2{ windows_system_->GetWinWidth(),
                           windows_system_->GetWinHeight() };

    glm::mat3 view_xform_ { 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos_.x , cam_pos_.y , 1 };

    // compute other matrices ...
    glm::mat3 camwin_to_ndc_xform_ { 2 / cam_size_.x , 0 , 0,
                                     0 , 2 / cam_size_.y , 0,
                                     0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;

    cam_zoom_ = 1.0f;
}

void CameraSystem::Update(float frametime)
{
    /*
    for (CameraIt it = camera_arr_.begin(); it != camera_arr_.end(); ++it) {

        if (debug_) {
            // Log id of entity and it's updated components that are being updated
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Camera updated)\n");
        }

        CameraUpdate(it->second);
    }
    */

    TempCameraUpdate();
}

void CameraSystem::Draw()
{
}

std::string CameraSystem::GetName()
{
	return "CameraSystem";
}

void CameraSystem::SendMessageD(Message* m)
{
}

void CameraSystem::TempCameraUpdate()
{
    glm::mat3 view_xform_{ 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos_.x , cam_pos_.y , 1 };

    // compute other matrices ...
    glm::mat3 camwin_to_ndc_xform_{ 2 / cam_size_.x , 0 , 0,
                                     0 , 2 / cam_size_.y , 0,
                                     0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void CameraSystem::CameraUpdate(Camera* camera)
{
    glm::mat3 view_xform_ { 1 , 0 , 0,
                            0 , 1 , 0,
                            camera->cam_pos_.x , camera->cam_pos_.y , 1 };

    // compute other matrices ...
    glm::mat3 camwin_to_ndc_xform_ { 2 / camera->cam_size_.x , 0 , 0,
                                     0 , 2 / camera->cam_size_.y , 0,
                                     0 , 0 , 1 };

    camera->world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void CameraSystem::CameraZoom(Camera* camera, float zoom)
{
    camera->cam_size_ *= zoom;
}

void CameraSystem::CameraMove(Camera* camera, Vector2D displacement)
{
    camera->cam_pos_.x -= displacement.x;
    camera->cam_pos_.y -= displacement.y;
}

void CameraSystem::TempCameraZoom(float zoom)
{
    cam_zoom_ *= zoom;

    cam_size_ = cam_zoom_ * glm::vec2{ windows_system_->GetWinWidth(),
                                       windows_system_->GetWinHeight() };
}

void CameraSystem::TempCameraMove(Vector2D displacement)
{
    cam_pos_.x -= displacement.x;
    cam_pos_.y -= displacement.y;
}