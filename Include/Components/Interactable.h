/**********************************************************************************
*\file         Interactable.h
*\brief        Contains declaration of functions and variables used for
*			   the Interactable Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _INTERACTABLE_H_
#define _INTERACTABLE_H_

#include <memory>
#include <string>
#include <sstream>
#include <map>
#include "MathLib/Vector2D.h"
#include "Components/IComponent.h"

enum class InteractableType
{
	NONE = 0,
	LOG
};


class Interactable : public Component {

public:

	/******************************************************************************/
	/*!
	  \fn Interactable()

	  \brief Constructor for Interactable that defaults the data members of the
			 component
	*/
	/******************************************************************************/
	Interactable();

	/******************************************************************************/
	/*!
	  \fn ~Interactable()

	  \brief Destructor for Interactable that removes the component
	*/
	/******************************************************************************/
	~Interactable();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initializes the Interactable component
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

	std::string GetAnimationName(const std::string&);

private:

	// Event, Animation Key Name
	std::map<std::string, std::string> event_animations_;
};


#endif