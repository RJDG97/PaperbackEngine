/**********************************************************************************
*\file         Camera.cpp
*\brief        Contains definition of functions and variables used for
*			   the Camera Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Camera.h"
#include "Engine/Core.h"
#include "Systems/CameraSystem.h"

Camera::Camera()
{
}

Camera::~Camera() {

    CORE->GetManager<ComponentManager>()->RemoveComponent<Camera>(Component::GetOwner()->GetID());
}

void Camera::Init()
{
    CORE->GetManager<ComponentManager>()->AddComponent<Camera>(Component::GetOwner()->GetID(), this);

    Vector2D cam_pos = CORE->GetManager<ComponentManager>()->GetComponent<Transform>(
                            this->GetOwner()->GetID())->GetPosition();

    glm::mat3 view_xform = { 1 , 0 , 0,
                             0 , 1 , 0,
                             -cam_pos.x , -cam_pos.y , 1 };

    glm::mat3 camwin_to_ndc_xform { 2 / cam_size_.x , 0 , 0,
                                    0 , 2 / cam_size_.y , 0,
                                    0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform * view_xform;
}

void Camera::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->StartObject();

    writer->Key("component");
    writer->String("Camera");

    writer->Key("Camera Size");
    writer->String((std::to_string(cam_size_.x) + " " + std::to_string(cam_size_.y)).c_str());

    writer->Key("Camera Zoom");
    writer->String(std::to_string(cam_zoom_).c_str());

    writer->EndObject();
}

void Camera::DeSerialize(std::stringstream& data)
{
    data >> cam_size_.x >> cam_size_.y
         >> cam_zoom_;

}

void Camera::DeSerializeClone(std::stringstream& data)
{
    DeSerialize(data);

    cam_size_ /= cam_zoom_;

    Vector2D cam_pos = CORE->GetManager<ComponentManager>()->GetComponent<Transform>(
                            this->GetOwner()->GetID())->GetPosition();

    glm::mat3 view_xform = { 1 , 0 , 0,
                             0 , 1 , 0,
                             -cam_pos.x , -cam_pos.y , 1 };

    glm::mat3 camwin_to_ndc_xform { 2 / cam_size_.x , 0 , 0,
                                    0 , 2 / cam_size_.y , 0,
                                    0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform * view_xform;
}

std::shared_ptr<Component> Camera::Clone()
{
    M_DEBUG->WriteDebugMessage("Cloning Health Component\n");

    std::shared_ptr<Camera> cloned = std::make_shared<Camera>();

    Vector2D cam_pos = std::reinterpret_pointer_cast<Transform>(
                        this->GetOwner()->GetComponent(ComponentTypes::TRANSFORM))->GetPosition();

    cloned->cam_zoom_ = cam_zoom_;
    cloned->cam_size_ = cam_size_ / cam_zoom_;

    glm::mat3 view_xform = { 1 , 0 , 0,
                             0 , 1 , 0,
                             -cam_pos.x , -cam_pos.y , 1 };

    glm::mat3 camwin_to_ndc_xform { 2 / cam_size_.x , 0 , 0,
                                    0 , 2 / cam_size_.y , 0,
                                    0 , 0 , 1 };

    cloned->world_to_ndc_xform_ = camwin_to_ndc_xform * view_xform;

    return cloned;
}

float* Camera::GetCameraZoom()
{
    return &cam_zoom_;
}

glm::mat3* Camera::GetCameraWorldToNDCTransform()
{
    return &world_to_ndc_xform_;
}

void Camera::SetCameraZoom(Camera* camera, float zoom) {

    camera->cam_zoom_ /= (zoom);

    camera->cam_size_ = glm::vec2{ CORE->GetSystem<WindowsSystem>()->GetWinWidth(),
                            CORE->GetSystem<WindowsSystem>()->GetWinHeight() } / camera->cam_zoom_;

}
