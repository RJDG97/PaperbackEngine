#pragma once
#ifndef _ANOTHER_WINDOW_H_
#define _ANOTHER_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Engine/Core.h"
#include "Manager/ForcesManager.h"


class AnotherWindow : public IWindow{

public:
	void Init() override;
	void Update() override;

	//void AddTransformComponent(EntityID id, Transform* scale);

	//void RemoveTransformComponent(EntityID id);

private:
using TransformIt = std::unordered_map<EntityID, Transform*>::iterator;
std::unordered_map<EntityID, Transform*> transform_arr_;
//
using MotionIt = std::unordered_map<EntityID, Motion*>::iterator;
std::unordered_map<EntityID, Motion*> motion_arr_;

Vector2D scale_;
};
#endif
