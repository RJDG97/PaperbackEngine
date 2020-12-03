/**********************************************************************************
*\file         Camera.h
*\brief        Contains declaration of functions and variables used for
*			   the Camera System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
	ComponentManager* component_manager_;

public:

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
	void CameraUpdate(Camera* camera);

/******************************************************************************/
/*!
	\fn CameraZoom()

	\brief Sets zoom data member of camera
*/
/******************************************************************************/
	void CameraZoom(Camera* camera, float zoom);

/******************************************************************************/
/*!
	\fn CameraMove()

	\brief Moves the camera
*/
/******************************************************************************/
	void CameraMove(Camera* camera, Vector2D displacement);

/******************************************************************************/
/*!
	\fn CameraSetPosition()

	\brief Sets postion of the camera
*/
/******************************************************************************/
	void CameraSetPosition(Camera* camera, Vector2D postion);

/******************************************************************************/
/*!
	\fn GetMainCamera()

	\brief Gets the main camera
*/
/******************************************************************************/
	Camera* GetMainCamera();
};

#endif