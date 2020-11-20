#pragma once
#ifndef _LOGIC_COMPONENT_H_
#define _LOGIC_COMPONENT_H_

#include <map>
#include "Entity/Entity.h"
#include "Components/IComponent.h"

class LogicComponent : public Component
{
public:
	// Function pointer called "LogicUpdate" that takes an entity id (of parent)
	// and a pointer to the component manager to avoid dynamic_pointer_cast
	using LogicUpdate = void(*)(const EntityID& parent_id);
	using LogicMap = std::map<std::string, LogicUpdate>;

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
	std::string entity_;
	LogicMap my_logic_;
	friend class Physics; //temp
};

#endif // !_LOGIC_COMPONENT_H_