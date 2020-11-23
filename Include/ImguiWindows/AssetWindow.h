#pragma once
#ifndef _ASSET_WINDOW_H_
#define _ASSET_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include <filesystem>
#include "Systems/Debug.h"
#include "Manager/TextureManager.h"
namespace fs = std::filesystem;


struct TextureInfo {

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

	void FileDirectoryCheck(fs::path filedirectory);

	void CheckFileType();

	void MakeNewFolder();

	void FileMenuBar();
	
	void DeleteWholeFolder();

	void DisplayFolders(float window_width, float window_height);

	void DisplayFolderFiles(float window_width, float window_height);

	void DeSerializeJSON(const std::string& filename, rapidjson::Document& doc);

	void LoadTextureJson(std::string level_name);
	void LoadAnimationJson(std::string level_name);

	std::string FileString(std::string icon, std::string file_name);
	std::string DirectoryName(fs::directory_entry directory);


private:

	ImguiSystem* imgui_;
	TextureManager* texture_;

	fs::path path_selection_;

	std::string folder_to_del_;
	std::string img_to_add_;
	std::string selected_file_;
	std::string chosen_json_;

	bool b_create, b_makefolder, b_deletefolder, b_addtexture, b_tex, b_anim;

	std::map<std::string, TextureInfo> tex_info_;


};
#endif
