/**********************************************************************************
*\file         AssetWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the AssetWindow for the Editor's Asset Browser

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _ASSET_WINDOW_H_
#define _ASSET_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include <filesystem>
#include "Manager/TextureManager.h"
#include "ImguiWindows\AssetConsoleWindow.h"

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
	/*****************************************************************************/
	void DisplayFolderFiles(float window_width, float window_height);

	/******************************************************************************/
	/*!
		\fn MultiFileSelection(std::string appended_files)

		\brief Handles the tokenizing and appending of file paths to the
			   directory path
	*/
	/******************************************************************************/
	std::vector<std::string> MultiFileSelection(std::string appended_files);

	/******************************************************************************/
	/*!
		\fn FileString(std::string icon, std::string file_name)

		\brief Appends a blank space between 2 strings
	*/
	/******************************************************************************/
	std::string FileString(std::string icon, std::string file_name);

	/******************************************************************************/
	/*!
		\fn DirectoryName(fs::directory_entry directory)

		\brief Gets the actual name of the Folder/Directory
	*/
	/******************************************************************************/
	std::string DirectoryName(fs::directory_entry directory);

	/******************************************************************************/
	/*!
		\fn CopyFilesDirectory()

		\brief Copies single or multiple files from the source directory
			   to the current directory
	*/
	/******************************************************************************/
	void CopyFilesDirectory();

	/******************************************************************************/
	/*!
		\fn FileDeletion()

		\brief Handles of deleting and unloading files from the asset browser
	*/
	/******************************************************************************/
	void FileDeletion();

private:

	ImguiSystem* imgui_;
	TextureManager* texture_;

	fs::path path_selection_;

	std::string folder_to_del_;
	std::string selected_file_;

	bool b_create, b_makefolder, b_deletefolder;

	std::vector<std::string> multifiles_;

	AssetConsoleWindow* asset_;
};
#endif
