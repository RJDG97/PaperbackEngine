#pragma once
#ifndef _CAMERASYSTEM_H_
#define _CAMERASYSTEM_H_

#include "Systems/ISystem.h"
#include "Components/Camera.h"
#include "Systems/WindowsSystem.h"
#include <unordered_map>

class CameraSystem : public ISystem {

	bool debug_;

	/* DONT USE THIS PART FOR NOW
	using CameraIt = std::unordered_map<EntityID, Camera*>::iterator;
	std::unordered_map<EntityID, Camera*> camera_arr_;
	*/

	WindowsSystem* windows_system_;

public:

	glm::vec2 cam_pos_;
	glm::vec2 cam_size_;
	glm::mat3 world_to_ndc_xform_;

	float cam_zoom_;

	void Init();

	void Update(float frametime);

	void Draw();

	std::string GetName();

	void SendMessageD(Message* m);

	~CameraSystem() = default;

	void TempCameraUpdate();

	void CameraUpdate(Camera* camera);

	void CameraZoom(Camera* camera, float zoom);

	void CameraMove(Camera* camera, Vector2D displacement);

	void TempCameraZoom(float zoom);

	void TempCameraMove(Vector2D displacement);
};

#endif