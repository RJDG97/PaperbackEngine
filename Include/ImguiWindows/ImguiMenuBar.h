#pragma once
#ifndef _IMGUI_MENU_BAR_H_
#define _IMGUI_MENU_BAR_H_

#include <string>

#include "ImguiWindows/IWindow.h"
#include "Systems/WindowsSystem.h"

class ImguiMenuBar : public IWindow{

    public:
/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the ImGui Viewport Window
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn Update()

	\brief Updates the ImGui Viewport
*/
/******************************************************************************/
	void Update() override;

/******************************************************************************/
/*!
	\fn OpenSaveDialog(const char* filter, int save)

	\brief Opens or Saves a file depending on the number being passed in
*/
/******************************************************************************/
    std::string OpenSaveDialog(const char* filter, int save);

    private:
    WindowsSystem* win;
};

#endif