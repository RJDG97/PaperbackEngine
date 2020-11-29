#include "Systems/CameraSystem.h"
#include "Engine/Core.h"
#include "Manager/EntityManager.h"

void CameraSystem::Init()
{
    component_manager_ = &*CORE->GetManager<ComponentManager>();
    camera_arr_ = component_manager_->GetComponentArray<Camera>();
    windows_system_ = &*CORE->GetSystem<WindowsSystem>();
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

        case MessageIDTypes::FLIP_SPRITE_X: {

            Camera* camera = GetMainCamera();
            CameraZoom(camera, camera->cam_zoom_ + 0.1f);
            break;
        }

        case MessageIDTypes::FLIP_SPRITE_Y: {

            Camera* camera = GetMainCamera();
            CameraZoom(camera, camera->cam_zoom_ - 0.1f);
            break;
        }
    }
}

void CameraSystem::CameraUpdate(Camera* camera)
{
    const float global_scale = CORE->GetGlobalScale();
    Vector2D position = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID())->GetPosition() * -1 * global_scale;
    camera->cam_pos_ = glm::vec2{ position.x, position.y };

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

void CameraSystem::CameraSetPosition(Camera* camera, Vector2D postion)
{
    camera->cam_pos_ = glm::vec2{ postion.x, postion.y };
}

Camera* CameraSystem::GetMainCamera()
{
    if (camera_arr_->size() == 0)
    {
        return nullptr;
    }

    return camera_arr_->begin()->second;
}