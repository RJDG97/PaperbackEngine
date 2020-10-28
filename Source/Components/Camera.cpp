#include "Components\Camera.h"
#include "Systems/Debug.h"

Camera::Camera()
{
}

void Camera::Init()
{
    glm::mat3 view_xform_ = { 1 , 0 , 0,
                              0 , 1 , 0,
                              cam_pos_.x , cam_pos_.y , 1 };

    glm::mat3 camwin_to_ndc_xform_ { 2 / cam_size_.x , 0 , 0,
                                     0 , 2 / cam_size_.y , 0,
                                     0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void Camera::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
}

void Camera::DeSerialize(std::stringstream& data)
{
    data >> cam_pos_.x >> cam_pos_.y
         >> cam_size_.x >> cam_size_.y;

}

void Camera::DeSerializeClone(std::stringstream& data)
{
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

    return cloned;
}
