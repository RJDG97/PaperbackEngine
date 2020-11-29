#pragma once
#include "Systems/ISystem.h"
#include "Manager/ComponentManager.h"
#include "Manager/DialogueManager.h"

class DialogueSystem : public ISystem
{
	enum DialogueStatus {

		INACTIVE,
		OPENING,
		ADVANCING,
		FINISHED_ADVANCING,
		CLOSING

	};

	DialogueManager* dialogue_manager_;

	Dialogue* current_dialogue_;
	std::vector<DialogueContent>::iterator current_dialogue_content_;
	std::string* entire_speech_;
	std::string current_speech_;
	float text_elapsed_time_;
	float text_speed_;

	float textbox_scale_;
	float textbox_scale_speed_;
	DialogueStatus dialogue_status_;

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
};