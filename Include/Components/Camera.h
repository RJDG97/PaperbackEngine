#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "IComponent.h"
#include "Entity/Entity.h"
#include <list>
#include <glm/glm.hpp>

// Contains data about the health of an entity that the component will be attached to
class Camera : public Component
{
	glm::vec2 cam_pos_;
	glm::vec2 cam_size_;

	glm::mat3 world_to_ndc_xform_;

public:

	friend class CameraSystem;

	Camera();

	void Init();

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

	void DeSerialize(std::stringstream& data) override;

	void DeSerializeClone(std::stringstream& data);

	std::shared_ptr<Component> Clone() override;
};

#endif