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
	std::string portrait_;
	std::string name_;
	std::string speech_;

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

  \brief Constructor for DiglogueContent
*/
/******************************************************************************/
	DialogueContent(std::string portrait, std::string name, std::string speech);

/******************************************************************************/
/*!
  \fn GetSpeech()

  \brief Get speech
*/
/******************************************************************************/
	std::string* GetSpeech();
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

  \brief Constructor for DiglogueContent
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

  \brief Load a full dialogue
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
