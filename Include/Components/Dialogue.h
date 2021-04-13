/**********************************************************************************
*\file         Dialogue.h
*\brief        Contains declaration of functions and variables used for
*			   the DialogueTrigger Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _DIALOGUE_TRIGGER_H_
#define _DIALOGUE_TRIGGER_H_

#include "Entity/Entity.h" 
#include "IComponent.h"
#include <sstream>
#include <memory>

class DialogueTrigger : public Component {

	std::string dialogue_name_;

public:

/******************************************************************************/
/*!
  \fn Dialogue()

  \brief Constructor for DialogueTrigger that defaults the data members of the
		 component
*/
/******************************************************************************/
	DialogueTrigger();

/******************************************************************************/
/*!
  \fn ~Dialogue()

  \brief Destructor for DialogueTrigger that removes the component from the
		 component manager
*/
/******************************************************************************/
	~DialogueTrigger();

/******************************************************************************/
/*!
  \fn GetDialogueName()

  \brief Returns the name of the dialogue to trigger
*/
/******************************************************************************/
	std::string GetDialogue() const;

/******************************************************************************/
/*!
  \fn SetDialogueName()

  \brief Sets the dialogue name of the component
*/
/******************************************************************************/
	void SetDialogueName(const std::string& dialogue_name);

/******************************************************************************/
/*!
  \fn Init()

  \brief Adds the component itself to the Collision system Dialogue map
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn DeSerializeClone()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;
};

#endif