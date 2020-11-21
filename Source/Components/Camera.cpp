#include "Components\Camera.h"
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
    targeted_ = false;

    CORE->GetManager<ComponentManager>()->AddComponent<Camera>(Component::GetOwner()->GetID(), this);

    glm::mat3 view_xform_ = { 1 , 0 , 0,
                              0 , 1 , 0,
                              cam_pos_.x , cam_pos_.y , 1 };

    glm::mat3 camwin_to_ndc_xform_ { 2 / (cam_size_.x / cam_zoom_) , 0 , 0,
                                     0 , 2 / (cam_size_.y / cam_zoom_) , 0,
                                     0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void Camera::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->StartObject();

    writer->Key("component");
    writer->String("Camera");

    writer->Key("Camera Position");
    writer->String((std::to_string(cam_pos_.x) + " " + std::to_string(cam_pos_.y)).c_str());

    writer->Key("Camera Size");
    writer->String((std::to_string(cam_size_.x) + " " + std::to_string(cam_size_.y)).c_str());

    writer->Key("Camera Zoom");
    writer->String(std::to_string(cam_zoom_).c_str());

    writer->EndObject();
}

void Camera::DeSerialize(std::stringstream& data)
{
    data >> cam_pos_.x >> cam_pos_.y
         >> cam_size_.x >> cam_size_.y
         >> cam_zoom_
         >> targeted_;

}

void Camera::DeSerializeClone(std::stringstream& data)
{
    DeSerialize(data);

    cam_size_ /= cam_zoom_;

    glm::mat3 view_xform_ = { 1 , 0 , 0,
                              0 , 1 , 0,
                              cam_pos_.x , cam_pos_.y , 1 };

    glm::mat3 camwin_to_ndc_xform_{ 2 / cam_size_.x , 0 , 0,
                                     0 , 2 / cam_size_.y , 0,
                                     0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

std::shared_ptr<Component> Camera::Clone()
{
    M_DEBUG->WriteDebugMessage("Cloning Health Component\n");

    std::shared_ptr<Camera> cloned = std::make_shared<Camera>();

    cloned->cam_pos_ = cam_pos_;
    cloned->cam_size_ = cam_size_;

    glm::mat3 view_xform_ = { 1 , 0 , 0,
                              0 , 1 , 0,
                              cam_pos_.x , cam_pos_.y , 1 };

    glm::mat3 camwin_to_ndc_xform_{ 2 / cam_size_.x , 0 , 0,
                                     0 , 2 / cam_size_.y , 0,
                                     0 , 0 , 1 };

    cloned->world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
    cloned->cam_zoom_ = cam_zoom_;

    return cloned;
}

glm::vec2* Camera::GetCameraPosition()
{
    return &cam_pos_;
}

Vector2D Camera::GetVector2DCameraPosition() {
    
    Vector2D position{ static_cast<float>(-cam_pos_.x), static_cast<float>(-cam_pos_.y) };

    return position;
}

float* Camera::GetCameraZoom()
{
    return &cam_zoom_;
}

glm::mat3* Camera::GetCameraWorldToNDCTransform()
{
    return &world_to_ndc_xform_;
}
