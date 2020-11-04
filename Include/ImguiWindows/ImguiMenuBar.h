#pragma once
#ifndef _IMGUI_MENU_BAR_H_
#define _IMGUI_MENU_BAR_H_

#include <string>

#include "ImguiWindows/IWindow.h"
#include "Systems/WindowsSystem.h"
#include "Systems/ImguiSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/Factory.h"


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

	void OpenFile();
	void SaveFile();

	void ImguiInput();
	int GetKey(ImGuiKey imguikey);

	void SaveArchetype();

    private:
    WindowsSystem* win_;
	ImguiSystem* imgui_system_;
	InputSystem* input_;
	const char* file_filter_;
	EntityFactory* factory_;
};

#endif