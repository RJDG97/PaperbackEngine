/**********************************************************************************
*\file         SystemWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the SystemWindow class to display percentages used by each system

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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

