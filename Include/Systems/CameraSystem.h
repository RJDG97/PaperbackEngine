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

	void Init();

	void Update(float frametime);

	void Draw();

	void AddCameraComponent(EntityID id, Camera* camera);

	void RemoveCameraComponent(EntityID id);

	void SendMessageD(Message* m);

	std::string GetName();

	~CameraSystem() = default;

	void CameraUpdate(/*Camera* camera*/);

	void CameraZoom(/*Camera* camera,*/ float zoom);

	void CameraMove(/*Camera* camera,*/ Vector2D displacement);

	void CameraSetPosition(/*Camera* camera,*/ Vector2D postion);

	void CameraUnTarget(/*Camera* camera,*/);

	void SetTarget(/*Camera* camera,*/ Entity* target);

	void ToggleTargeted(/*Camera* camera*/);
};

#endif