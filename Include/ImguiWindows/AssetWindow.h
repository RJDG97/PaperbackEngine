#pragma once
#ifndef _ASSET_WINDOW_H_
#define _ASSET_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include <filesystem>
namespace fs = std::filesystem;
class AssetWindow : public IWindow {

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

	void FileDirectoryCheck(fs::path directory);
private:
	ImguiSystem* imgui_;

};
#endif
