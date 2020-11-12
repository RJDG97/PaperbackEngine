#pragma once
#ifndef _CAMERASYSTEM_H_
#define _CAMERASYSTEM_H_

#include "Systems/ISystem.h"
#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Systems/WindowsSystem.h"
#include "Manager/ComponentManager.h"
#include <unordered_map>

class CameraSystem : public ISystem {

	bool debug_;

	using CameraType = CMap<Camera>;
	using CameraIt = CameraType::MapTypeIt;
	CameraType* camera_arr_;

	WindowsSystem* windows_system_;

	//Temporary, will make use of camera components next time
	Transform* target_;
	bool targeted_;

public:

	//Temporary, will make use of camera components next time
	glm::vec2 cam_pos_;
	glm::vec2 cam_size_;
	glm::mat3 world_to_ndc_xform_;
	float cam_zoom_;

/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the Camera System
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
	\fn Update()

	\brief Updates all camera components
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
	\fn Draw()

	\brief Currently not used
*/
/******************************************************************************/
	void Draw();


/******************************************************************************/
/*!
	\fn AddCameraComponent()

	\brief Adds a Camera component to the camera map
*/
/******************************************************************************/
	void AddCameraComponent(EntityID id, Camera* camera);

/******************************************************************************/
/*!
	\fn RemoveCameraComponent()

	\brief Removes a Camera component from the camera map
*/
/******************************************************************************/
	void RemoveCameraComponent(EntityID id);

/******************************************************************************/
/*!
	\fn SendMessageD()

	\brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	void SendMessageD(Message* m);

/******************************************************************************/
/*!
	\fn GetName()

	\brief Returns the name of the system for debug use
*/
/******************************************************************************/
	std::string GetName();

/******************************************************************************/
/*!
	\fn ~CameraSystem()

	\brief Destructor for Camera System
*/
/******************************************************************************/
	~CameraSystem() = default;

/******************************************************************************/
/*!
	\fn CameraUpdate()

	\brief Updates camera
*/
/******************************************************************************/
	void CameraUpdate(/*Camera* camera*/);

/******************************************************************************/
/*!
	\fn CameraZoom()

	\brief Sets zoom data member of camera
*/
/******************************************************************************/
	void CameraZoom(/*Camera* camera,*/ float zoom);

/******************************************************************************/
/*!
	\fn CameraMove()

	\brief Moves the camera
*/
/******************************************************************************/
	void CameraMove(/*Camera* camera,*/ Vector2D displacement);

/******************************************************************************/
/*!
	\fn CameraSetPosition()

	\brief Sets postion of the camera
*/
/******************************************************************************/
	void CameraSetPosition(/*Camera* camera,*/ Vector2D postion);

/******************************************************************************/
/*!
	\fn CameraUnTarget()

	\brief Sets target of camera to nullptr and un-snaps camera from target
*/
/******************************************************************************/
	void CameraUnTarget(/*Camera* camera,*/);

/******************************************************************************/
/*!
	\fn CameraUnTarget()

	\brief Sets target of camera to the entity
*/
/******************************************************************************/
	void SetTarget(/*Camera* camera,*/ Entity* target);

/******************************************************************************/
/*!
	\fn ToggleTargeted()

	\brief Toggles whether camera follow target or moves independantly from
		   the entity
*/
/******************************************************************************/
	void ToggleTargeted(/*Camera* camera*/);
};

#endif