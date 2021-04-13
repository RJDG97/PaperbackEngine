/**********************************************************************************
*\file         IWindow.h
*\brief        Contains declaration of the IWindow interface class used for
*			   all Editor windows

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _IWINDOW_H_
#define _IWINDOW_H_

// imgui headers
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

/******************************************************************************/
/*!
	\fn ~IWindow() = default

	\brief Virtual destructor for all Iwindow derived class
*/
/******************************************************************************/
		virtual ~IWindow() = default;
};

#endif