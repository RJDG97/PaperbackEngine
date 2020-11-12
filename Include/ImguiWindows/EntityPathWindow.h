#pragma once
#ifndef _ENTITY_PATH_WINDOW_H_
#define _ENTITY_PATH_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "GameStates/Levels.h"
#include "Manager/EntityManager.h"
#include "Systems/Factory.h"

class EntityPathWindow : public IWindow {
  
public:

	void Init() override;
	void Update() override;

private:

	ImguiSystem* imgui_;
	EntityManager* entities_;
	EntityFactory* factory_;

};
#endif