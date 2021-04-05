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
#include <random>

class CameraSystem : public ISystem {

	bool debug_;

	using CameraType = CMap<Camera>;
	using CameraIt = CameraType::MapTypeIt;
	CameraType* camera_arr_;

	WindowsSystem* windows_system_;
	ComponentManager* component_manager_;

	Vector2D win_size_;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution { -15.0f, 15.0f };

	struct Shake
	{
		float duration_;
		float elapsed_time_;
		float amplitude_;

		Shake() = default;
		Shake(float duration, float amplitude);
	};

	std::vector<Shake> shakes_; // first is duration and second is magnitude
	float shake_angle;
	float shake_angle_timer;					  // will change angle whenever when timer is 0
	Vector2D shake_offset {0.0f, 0.0f};
	float total_magnitude;

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
	\fn CameraMove(Camera* camera, Vector2D displacement)

	\brief Moves the camera
*/
/******************************************************************************/
	void CameraMove(Camera* camera, Vector2D displacement);

/******************************************************************************/
/*!
	\fn CameraSetPosition(Camera* camera, Vector2D position)

	\brief Sets postion of the camera
*/
/******************************************************************************/
	void CameraSetPosition(Camera* camera, Vector2D position);

/******************************************************************************/
/*!
	\fn GetMainCamera()

	\brief Gets the main camera
*/
/******************************************************************************/
	Camera* GetMainCamera();

/******************************************************************************/
/*!
	\fn GetMainCameraPos()

	\brief Gets the position of the main camera
*/
/******************************************************************************/
	Vector2D GetMainCameraPos();

	Vector2D UIToGameCoords(const Vector2D & ui_pos);

	Vector2D GameCoordsToUI(const Vector2D & go_pos);

	void ScreenShake(float duration, float magnitude);

	void TargetPlayer();

	void TargetVector(Vector2D* target);
};

#endif