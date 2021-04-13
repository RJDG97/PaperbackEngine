/**********************************************************************************
*\file         LogicComponent.h
*\brief        Contains declaration of functions and variables used for
*			   the Logic Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _LOGIC_COMPONENT_H_
#define _LOGIC_COMPONENT_H_

#include <map>
#include "Entity/Entity.h"
#include "Components/IComponent.h"


class LogicComponent : public Component
{
public:
	
	using LogicMap = std::map<std::string, std::string>;


/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serialises a component as a clone into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Loads a component from JSON format
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data);

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Retrieves data that will initialise data that can be unique from other
		 components
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data);

/******************************************************************************/
/*!
  \fn GetLogic()

  \brief Retrieves the relevant string for update fn
*/
/******************************************************************************/
	const std::string GetLogic(const std::string& fn);

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone();

/***************************************************************************/
/*!
  \fn ~LogicComponent()

  \brief Removes the component from the component map
*/
/***************************************************************************/
	~LogicComponent();

private:

	size_t size_;
	LogicMap my_logic_;

	friend class Physics; //temp
};

#endif // !_LOGIC_COMPONENT_H_