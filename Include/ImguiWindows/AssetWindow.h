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

	void FileDirectoryCheck(fs::path filedirectory);

	void CheckFileType();

	void MakeNewFolder();

	void FileMenuBar();
	
	void DeleteWholeFolder();

	void DisplayFolders(float window_width, float window_height);

	void DisplayFolderFiles(float window_width, float window_height);

	std::string FileString(std::string icon, std::string file_name);
	std::string DirectoryName(fs::directory_entry directory);

private:
	ImguiSystem* imgui_;
	fs::path path_selection_;
	std::string folder_to_del;
	
	std::string selected_file_;
	bool b_create, b_makefolder, b_deletefolder;

};
#endif
