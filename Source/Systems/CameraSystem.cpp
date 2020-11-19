#include "Systems/CameraSystem.h"
#include "Engine/Core.h"
#include "Manager/EntityManager.h"

void CameraSystem::Init()
{
    windows_system_ = &*CORE->GetSystem<WindowsSystem>();

    cam_zoom_ = 0.8f;
    targeted_ = false;

    cam_pos_ = glm::vec2{ 0, 0 };
    cam_size_ = glm::vec2{ windows_system_->GetWinWidth(),
                           windows_system_->GetWinHeight() } / cam_zoom_;

    glm::mat3 view_xform_{ 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos_.x , cam_pos_.y , 1 };

    // compute other matrices ...
    glm::mat3 camwin_to_ndc_xform_{ 2 / cam_size_.x , 0 , 0,
                                     0 , 2 / cam_size_.y , 0,
                                     0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void CameraSystem::Update(float frametime)
{
    (void)frametime;

    //for (CameraIt camera = camera_arr_->begin(); camera != camera_arr_->end(); ++camera) {

        //CameraUpdate(camera->second);
    //}

    CameraUpdate();
}

void CameraSystem::Draw()
{
}

void CameraSystem::AddCameraComponent(EntityID id, Camera* camera)
{
    M_DEBUG->WriteDebugMessage("Adding Text Renderer Component to entity: "
        + std::to_string(id) + "\n");

    //text_renderer_arr_[id] = text_renderer;
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

    switch (m->message_id_) {

    case MessageIDTypes::CAM_UPDATE_POS: {

        if (!targeted_)
        {
            MessagePhysics_Motion* msg = dynamic_cast<MessagePhysics_Motion*>(m);
            CameraMove(/*current_camera,*/ msg->new_vec_);
        }

        break;
    }

    case MessageIDTypes::CHANGE_ANIMATION_1: {

            //Temporary
            if (CORE->GetManager<EntityManager>()->GetPlayerEntities().size() > 0)
            {
                SetTarget(/*current_camera,*/ CORE->GetManager<EntityManager>()->GetPlayerEntities()[0]);
                ToggleTargeted(/*current_camera*/);
            }

            break;
        }

        case MessageIDTypes::FLIP_SPRITE_X: {

            CameraZoom(/*current_camera,*/ 0.9f);
            break;
        }

        case MessageIDTypes::FLIP_SPRITE_Y: {

            CameraZoom(/*current_camera,*/ 1.1f);
            break;
        }
    }
}

void CameraSystem::CameraUpdate(/*Camera* camera*/)
{
    if (/*camera->*/targeted_)
    {
        const float global_scale = CORE->GetGlobalScale();
        Vector2D target = /*camera->*/target_->GetPosition() * -1;
        /*camera->*/cam_pos_ = glm::vec2{ target.x, target.y } * global_scale;
    }

    glm::mat3 view_xform_ { 1 , 0 , 0,
                            0 , 1 , 0,
                            /*camera->*/cam_pos_.x , /*camera->*/cam_pos_.y , 1 };

    // compute other matrices ...
    glm::mat3 camwin_to_ndc_xform_ { 2 / /*camera->*/cam_size_.x , 0 , 0,
                                     0 , 2 / /*camera->*/cam_size_.y , 0,
                                     0 , 0 , 1 };

    /*camera->*/world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void CameraSystem::CameraZoom(/*Camera* camera,*/ float zoom)
{
    /*camera->*/cam_size_ *= zoom;
}

void CameraSystem::CameraMove(/*Camera* camera,*/ Vector2D displacement)
{
    if (!/*camera->*/targeted_)
    {
        /*camera->*/cam_pos_.x -= displacement.x;
        /*camera->*/cam_pos_.y -= displacement.y;
    }
}

void CameraSystem::CameraSetPosition(/*Camera* camera,*/ Vector2D postion)
{
    cam_pos_ = glm::vec2{ postion.x, postion.y };
}

void CameraSystem::CameraUnTarget()
{
    target_ = nullptr;
    targeted_ = false;
}

void CameraSystem::SetTarget(/*Camera* camera,*/ Entity* target)
{
    /*camera->*/target_ = &*std::dynamic_pointer_cast<Transform>(target->GetComponent(ComponentTypes::TRANSFORM));
}

void CameraSystem::ToggleTargeted(/*Camera* camera*/)
{
    /*camera->*/targeted_ = !/*camera->*/targeted_;
}
