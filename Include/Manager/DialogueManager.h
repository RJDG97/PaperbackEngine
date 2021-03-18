/**********************************************************************************
*\file         DialogueManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Dialogue Manager
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#include "Manager/IManager.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/document.h"
#include "prettywriter.h"
#include <vector>
#include <string>
#include <map>

class DialogueContent
{
	std::string left_portrait_;
	std::string right_portrait_;
	std::string name_;
	std::string speech_;
	bool left_darken_;
	bool right_darken_;

public:

/******************************************************************************/
/*!
  \fn DialogueContent()

  \brief Default constructor
*/
/******************************************************************************/
	DialogueContent() = default;

/******************************************************************************/
/*!
  \fn DialogueContent()

  \brief Constructor for DialogueContent
*/
/******************************************************************************/
	DialogueContent(std::string left_portrait,
					std::string right_portrait,
					std::string name,
					std::string speech,
					bool left_darken,
					bool right_darken);

/******************************************************************************/
/*!
  \fn GetSpeech()

  \brief Get speech
*/
/******************************************************************************/
	std::string* GetSpeech();

/******************************************************************************/
/*!
  \fn GetNameLeft()

  \brief Get left name of speech
*/
/******************************************************************************/
	std::string GetName();

/******************************************************************************/
/*!
	\fn GetPortraitLeft()

	\brief Get right name of speech
*/
/******************************************************************************/
	std::string GetPortraitLeft();

/******************************************************************************/
/*!
  \fn GetPortraitRight()

  \brief Gets portrait
*/
/******************************************************************************/
	std::string GetPortraitRight();

/******************************************************************************/
/*!
  \fn GetLeftDarken()

  \brief Checks if left portrait needs to be darkened
*/
/******************************************************************************/
	bool GetLeftDarken();

/******************************************************************************/
/*!
	\fn GetRightDarken()

	\brief Checks if right portrait needs to be darkened
*/
/******************************************************************************/
	bool GetRightDarken();
};

class Dialogue
{
	std::vector<DialogueContent> contents_;

public:

/******************************************************************************/
/*!
  \fn Dialogue()

  \brief Default constructor
*/
/******************************************************************************/
	Dialogue() = default;

/******************************************************************************/
/*!
  \fn Dialogue()

  \brief Constructor for Dialogue
*/
/******************************************************************************/
	Dialogue(std::vector<DialogueContent> contents);

/******************************************************************************/
/*!
  \fn GetContents()

  \brief Get contents of Diglogue
*/
/******************************************************************************/
	std::vector<DialogueContent>* GetContents();
};

class DialogueManager : public IManager {

	std::map<std::string, Dialogue> dialogues_;

public:

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize Dialogue Manager
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn LoadDialogueSet()

  \brief Load a full dialogue set
*/
/******************************************************************************/
	void LoadDialogueSet(std::string level_name);

/******************************************************************************/
/*!
  \fn LoadDialogue()

  \brief Load a dialogue
*/
/******************************************************************************/
	void LoadDialogue(std::string dialogue_name, std::string path);

/******************************************************************************/
/*!
  \fn DeserializeJSON()

  \brief Parse data
*/
/******************************************************************************/
	void DeserializeJSON(const std::string& filename, rapidjson::Document& doc);

/******************************************************************************/
/*!
  \fn GetDialogue()

  \brief Get the dialogue paired with the dialogue_name
*/
/******************************************************************************/
	Dialogue* GetDialogue(std::string dialogue_name);
};
