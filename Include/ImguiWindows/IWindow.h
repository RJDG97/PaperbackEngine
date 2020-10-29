#pragma once
#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

class IWindow
{
	public:
/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the ImGui window added to the system
*/
/******************************************************************************/
		virtual void Init() {};

/******************************************************************************/
/*!
	\fn Update()

	\brief Runs the imgui windows added to the system
*/
/******************************************************************************/
		virtual void Update() = 0;
		virtual ~IWindow() = default;
};

#endif