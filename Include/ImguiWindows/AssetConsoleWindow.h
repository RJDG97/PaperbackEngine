#pragma once
#ifndef _ASSET_CONSOLE_WINDOW_H_
#define _ASSET_CONSOLE_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/TextureManager.h"
#include <filesystem>

namespace fs = std::filesystem;

struct TextureInfo { // struct to store the infomation of the textures

	std::string path;
	int column;
	int row;

};

class AssetConsoleWindow : public IWindow {

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

	/******************************************************************************/
	/*!
		\fn LoadTextureJson(std::string level_name);

		\brief Load the json into a map
	*/
	/******************************************************************************/
	void LoadTextureJson(std::string level_name);

	void LoadAnimationJson(std::string level_name);

	void LoadSoundJson(std::string level_name);

	/******************************************************************************/
	/*!
		\fn AddTextureAnimation()

		\brief Handles the adding/removing of textures (the overall window)
	*/
	/******************************************************************************/
	void AddTextureAnimation();

	/******************************************************************************/
	/*!
		\fn SelectTextureJson()

		\brief Allows users to select which json they want to amend
	*/
	/******************************************************************************/
	void SelectTextureJson();

	/******************************************************************************/
	/*!
		\fn DisplayJson()

		\brief Displays the infomation of the chosen json
	*/
	/******************************************************************************/
	void DisplayJson();

	/******************************************************************************/
	/*!
		\fn AddNewTexture()

		\brief Adds a new texture to the map
	*/
	/******************************************************************************/
	void AddNewTexture();

	void AddBlankJson();

	std::string FindUnderscore(std::string filename);

private:
	ImguiSystem* imgui_;
	TextureManager* texture_;

	std::map<std::string, TextureInfo> tex_info_;
	std::vector<std::string> filesdel_;
	std::vector<std::string> jsonfiles_;
	std::string chosen_json_;

	bool b_unload;

};
#endif 

