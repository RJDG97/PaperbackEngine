#pragma once
#ifndef _ASSET_WINDOW_H_
#define _ASSET_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include <filesystem>
#include "Systems/Debug.h"
#include "Manager/TextureManager.h"

namespace fs = std::filesystem;

struct TextureInfo { // struct to store the infomation of the textures

	std::string path;
	int column;
	int row;

};

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

	/******************************************************************************/
	/*!
		\fn FileDirectoryCheck(fs::path filedirectory)

		\brief Recursively checks for folders and subfolders
	*/
	/******************************************************************************/
	void FileDirectoryCheck(fs::path filedirectory);
	/******************************************************************************/
	/*!
		\fn CheckFileType()

		\brief Checks the file type inside a directory and prints out the 
		corresponding images
	*/
	/******************************************************************************/
	void CheckFileType();

	/******************************************************************************/
	/*!
		\fn MakeNewFolder()

		\brief Creates a new folder in the current directory
	*/
	/******************************************************************************/
	void MakeNewFolder();

	/******************************************************************************/
	/*!
		\fn FileMenuBar()

		\brief Handles the menu bar of the asset browser
	*/
	/******************************************************************************/
	void FileMenuBar();

	/******************************************************************************/
	/*!
		\fn DeleteWholeFolder()

		\brief Handles the check for deleting non empty folders
	*/
	/******************************************************************************/
	void DeleteWholeFolder();

	/******************************************************************************/
	/*!
		\fn DisplayFolders(float window_width, float window_height)

		\brief Displays all folders of the "Resources" folder
	*/
	/******************************************************************************/

	void DisplayFolders(float window_width, float window_height);

	/******************************************************************************/
	/*!
		\fn DisplayFolderFiles(float window_width, float window_height)

		\brief Displays all folders and files inside a directory
	*/
	/******************************************************************************/

	void DisplayFolderFiles(float window_width, float window_height);

	/******************************************************************************/
	/*!
		\fn DeSerializeTextureJSON(const std::string& filename, rapidjson::Document& doc)

		\brief Deserialise the texture json
	*/
	/******************************************************************************/

	void DeSerializeTextureJSON(const std::string& filename, rapidjson::Document& doc);

	/******************************************************************************/
	/*!
		\fn LoadTextureJson(std::string level_name);

		\brief Load the json into a map
	*/
	/******************************************************************************/
	void LoadTextureJson(std::string level_name);

	void LoadAnimationJson(std::string level_name);

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

	std::string FileString(std::string icon, std::string file_name);
	std::string DirectoryName(fs::directory_entry directory);

	std::string FindUnderscore(std::string filename);

	std::vector<std::string> MultiFileSelection(std::string appended_files);
	void AddBlankJson();

private:

	ImguiSystem* imgui_;
	TextureManager* texture_;

	fs::path path_selection_;

	std::string folder_to_del_;
	std::string img_to_add_;
	std::string selected_file_;
	std::string chosen_json_;

	bool b_create, b_makefolder, b_deletefolder, b_tex, b_anim;

	std::map<std::string, TextureInfo> tex_info_;
	std::vector<std::string> multifiles_;
	std::vector<std::string> filesdel_;
	std::vector<std::string> jsonfiles_;


};
#endif
