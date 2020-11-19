#pragma once
#ifndef _SYSTEM_WINDOW_H_
#define _SYSTEM_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"

class SystemWindow : public IWindow {

public:
/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the ImGui System
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn Update()

	\brief Updates all ImGui Windows in the system
*/
/******************************************************************************/
	void Update() override;

private:
	ImguiSystem* imgui_;
};

#endif

