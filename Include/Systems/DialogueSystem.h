/**********************************************************************************
*\file         DialogueSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Dialogue System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#include "Systems/ISystem.h"
#include "Manager/ComponentManager.h"
#include "Manager/DialogueManager.h"
#include "Systems/SoundSystem.h"

class DialogueSystem : public ISystem
{
public:
	enum DialogueStatus {

		INACTIVE,
		OPENING,
		ADVANCING,
		FINISHED_ADVANCING,
		CLOSING

	};

private:
	DialogueManager* dialogue_manager_;
	SoundSystem* sound_system_;
	ComponentManager* component_manager_;
	GraphicsSystem* graphics_system_;

	Dialogue* current_dialogue_;
	std::vector<DialogueContent>::iterator current_dialogue_content_;
	std::string* entire_speech_;
	std::string current_speech_;
	float text_elapsed_time_;
	float text_speed_;
	size_t num_characters_;

	Vector2D textbox_max_scale_;
	float textbox_scale_speed_;
	DialogueStatus dialogue_status_;

	Scale* dialogue_box_scale_;
	TextureRenderer* dialogue_box_renderer_;
	TextRenderer* dialogue_text_renderer_;
	TextureRenderer* dialogue_portrait_renderer_left_;
	TextureRenderer* dialogue_portrait_renderer_right_;
	TextRenderer* dialogue_speakername_renderer_;

public:

/******************************************************************************/
/*!
	\fn Init()

	\brief Used to initialise a system if required;
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
	\fn Update()

	\brief Used to run logic that is required every game loop.
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
	\fn GetName()

	\brief Returns the name of the system
*/
/******************************************************************************/
	std::string GetName() { return "DialogueSystem"; }

/******************************************************************************/
/*!
	\fn SendMessageD()

	\brief Handles incoming messages and sorts based on message id
*/
/******************************************************************************/
	void SendMessageD(Message* m);

/******************************************************************************/
/*!
	\fn SetCurrentDialogue(std::string dialogue_name)

	\brief Sets the current dialogue
*/
/******************************************************************************/
	void SetCurrentDialogue(std::string dialogue_name);

/******************************************************************************/
/*!
	\fn AdvanceText()

	\brief Advances the text
*/
/******************************************************************************/
	void AdvanceText();

/******************************************************************************/
/*!
	\fn TempCleanup()

	\brief Temporary cleanup
*/
/******************************************************************************/
	void TempCleanup();

/******************************************************************************/
/*!
	\fn GetCurrentDialogueStatus()

	\brief Returns the current dialogue status
*/
/******************************************************************************/
	DialogueStatus GetCurrentDialogueStatus() const;

/******************************************************************************/
/*!
	\fn UpdatePortraits()

	\brief Handles the updating of portrait textures and alive bool
*/
/******************************************************************************/
	void UpdatePortraits();
};