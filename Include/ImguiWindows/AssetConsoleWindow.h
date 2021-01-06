/**********************************************************************************
*\file         AssetConsoleWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the AssetConsoleWindow for the management of assets added into the
*			   engine (Eg. Audio, Textures, Animation)

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _ASSET_CONSOLE_WINDOW_H_
#define _ASSET_CONSOLE_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "ImguiWindows/ArchetypeWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/TextureManager.h"
#include <filesystem>

namespace fs = std::filesystem;

struct TextureInfo { // struct to store the infomation of the textures

	std::string path;
	int column;
	int row;

};

struct AudioInfo {
	
	std::string path;
	float volume;
	float min_distance;
	float vol_falloff;
	int loop;
};

struct AnimationInfo {

	std::string anim_batch_name_;
	std::string anim_folder_;
	int column_;
	int row_;
};

struct IndividualAnimationInfo {

	std::string anim_name_;
	int num_frames_;
	GLfloat frame_duration_;
};

enum AddFile {

	ADDTEXTURE = 0,
	ADDAUDIO,
	ADDANIMATION,
	ADDNOTHING
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
		\fn AddTextureAnimation()

		\brief Handles the adding/removing of textures (the overall window)
	*/
	/******************************************************************************/
	void AddTextureAnimation();

	/******************************************************************************/
	/*!
		\fn LoadTextureJson(std::string level_name)

		\brief Load the json into a map
	*/
	/******************************************************************************/
	void LoadTextureJson(std::string level_name);

	/******************************************************************************/
	/*!
		\fn LoadAnimationJson(std::string level_name)

		\brief Load the json into a map
	*/
	/******************************************************************************/
	void LoadAnimationJson(std::string level_name);

	/******************************************************************************/
	/*!
		\fn LoadAnimationJson(std::string level_name)

		\brief Load the json into a map
	*/
	/******************************************************************************/
	void LoadAnimationsJson(std::string json_path);

	/******************************************************************************/
	/*!
		\fn LoadSoundJson(std::string level_name)

		\brief Load the json into a map
	*/
	/******************************************************************************/
	void LoadSoundJson(std::string level_name);

	/******************************************************************************/
	/*!
		\fn SelectTextureJson()

		\brief Allows users to select which json they want to amend
	*/
	/******************************************************************************/
	void SelectAssetJson();

	/******************************************************************************/
	/*!
		\fn DisplayTextureJson()

		\brief Displays the information of the chosen json
	*/
	/******************************************************************************/
	void DisplayTextureJson();

	/******************************************************************************/
	/*!
		\fn DisplayTextureJson()

		\brief Displays the information of the chosen json
	*/
	/******************************************************************************/
	void DisplayAnimationJson();

	/******************************************************************************/
	/*!
		\fn DisplayAudioJson()

		\brief Displays the information of the chosen json
	*/
	/******************************************************************************/
	void DisplayAudioJson();

	/******************************************************************************/
	/*!
		\fn AddNewAsset()

		\brief Adds a new Asset depending on the file type
	*/
	/******************************************************************************/
	void AddNewAsset();

	/******************************************************************************/
	/*!
		\fn UpdatePath()

		\brief Update the path of the texture using Drag and Drop within the editor
	*/
	/******************************************************************************/
	void UpdatePath();

	/******************************************************************************/
	/*!
		\fn FindUnderscore(std::string filename)

		\brief Gets the actual name of the json
	*/
	/******************************************************************************/
	std::string FindUnderscore(std::string filename);

	/******************************************************************************/
	/*!
		\fn WrongTypePopup()

		\brief Pop up to warn/inform users that they are trying to add wrong files
	*/
	/******************************************************************************/
	void WrongTypePopup();

	/******************************************************************************/
	/*!
		\fn UnloadPopup()

		\brief Pop up to info users that the textures have been unloaded
	*/
	/******************************************************************************/
	void UnloadPopup();

	/******************************************************************************/
	/*!
		\fn AddBlankJson()

		\brief Adds a blank json file
	*/
	/******************************************************************************/
	void AddBlankJson();
	/******************************************************************************/
	/*!
		\fn GetUnloadBool()

		\brief Gets the bool for the Unload pop up
	*/
	/******************************************************************************/
	bool GetUnloadBool();

	/******************************************************************************/
	/*!
		\fn SetUnloadBool(bool updated_bool)

		\brief Sets the bool for the Unload pop up
	*/
	/******************************************************************************/
	void SetUnloadBool(bool updated_bool);

	/******************************************************************************/
	/*!
		\fn SerializeJson()

		\brief Serialize data of the different assets into their respective jsons
	*/
	/******************************************************************************/
	void SerializeJson();

private:
	ImguiSystem* imgui_;
	TextureManager* texture_;
	SoundSystem* sound_;
	GraphicsSystem* graphics_;
	ComponentManager* component_;
	EntityManager* entities_;
	ArchetypeWindow* archetype_;
	
	std::map<std::string, TextureInfo> tex_info_;
	std::map<std::string, AudioInfo> audio_info;

	// key for the following maps would be the same one (spritesheet name)
	std::map<std::string, AnimationInfo> general_anim_info_; // store general infomation of the animation json
	std::map<std::string, std::vector<IndividualAnimationInfo>> animation_info_; // store specific infomation of the individual animation
	std::vector<IndividualAnimationInfo> indi_anim_info_;

	std::map<std::string, std::string> listAnimationJson_;;

	std::vector<std::string> filesdel_;
	std::vector<std::string> jsonfiles_;
	std::vector<EntityID> listEntity_;
	std::vector<std::string> listArchetype_;
	std::string chosen_json_;

	std::string json_to_load_; 
	std::string animation_json_;

	bool b_unload, b_wrong_type, b_load;

	AddFile type;

};
#endif 

