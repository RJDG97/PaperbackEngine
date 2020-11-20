#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "IComponent.h"
#include "Entity/Entity.h"
#include "Components/Transform.h"
#include <list>
#include <glm/glm.hpp>

// Contains data about the health of an entity that the component will be attached to
class Camera : public Component
{
	glm::vec2 cam_pos_;
	glm::vec2 cam_size_;
	float cam_zoom_;

	glm::mat3 world_to_ndc_xform_;

	Transform* target_;
	bool targeted_;

public:

	friend class CameraSystem;

/******************************************************************************/
/*!
	\fn Camera()

	\brief Constructor for Camera that defaults the data members of
			the component
*/
/******************************************************************************/
	Camera();

/******************************************************************************/
/*!
	\fn ~Camera()

	\brief Destructor for Camera that removes the component from the
			Camera System's camera map
*/
/******************************************************************************/
	~Camera();

/******************************************************************************/
/*!
	\fn Init()

	\brief Adds the component itself to the Camera System's camera map
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
	\fn Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)

	\brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
	\fn DeSerialize(std::stringstream& data)

	\brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
	\fn DeSerializeClone(std::stringstream& data)

	\brief Serializes data members within the Component
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data);

/******************************************************************************/
/*!
	\fn Clone()

	\brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

	glm::vec2 GetCamPos();
};

#endif