#pragma once
#ifndef _ANOTHER_WINDOW_H_
#define _ANOTHER_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include <Windows.h>
#include "Systems/ImguiSystem.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"


class AnotherWindow : public IWindow{

public:
	void Init() override;
	void Update() override;

/******************************************************************************/
/*!
	\fn ComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponents)

	\brief Prints out the different ImGui widgets required for each component type
*/
/******************************************************************************/
	void ComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponents);

/******************************************************************************/
/*!
	\fn ComponentInput(const char* componentLabel, const char* inputLabel,
	float& componentVar, float startVal = 0.1f, float endVal = 1.0f,
	float inputWidth = 110.0f)

	\brief Prints out ImGui input space for transformation components
*/
/******************************************************************************/
	void ComponentInput(const char* componentLabel, const char* inputLabel, float& componentVar, float startVal = 0.1f, float endVal = 1.0f, float inputWidth = 110.0f);


private:
	ImguiSystem* imgui_system_;

};
#endif
