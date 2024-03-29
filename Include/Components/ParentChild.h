/**********************************************************************************
*\file         ParentChild.h
*\brief        Contains declaration of functions and variables used for
*			   the ParentChild Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _PARENT_CHILD_H_
#define _PARENT_CHILD_H_

#include <list>
#include <string>
#include "Components/IComponent.h"

class ParentChild : public Component
{
private:
	// Note that every parent should have a unique name
	std::string name_; // Should be "" if the parent is not supposed to have a child
	std::list<Entity*> children_;

public:
	friend class LogicSystem;
	friend class Physics; //temp

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
	void Init() override;

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

  \brief Serialises a component as a clone into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
  \fn DeSerialize()

  \brief Loads a component from JSON format
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Retrieves data that will initialise data that can be unique from other
		 components
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

/******************************************************************************/
/*!
  \fn GetChildren()

  \brief Return a reference to a vector of children
*/
/******************************************************************************/
	std::list<Entity*>& GetChildren(); // Update all instances of GetChildren

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the parent
*/
/******************************************************************************/
	std::string GetName() const;

/******************************************************************************/
/*!
  \fn AddChild()

  \brief Attaches a child entity to parent
*/
/******************************************************************************/
	void AddChild(const EntityID& id);

/******************************************************************************/
/*!
  \fn RemoveChild()

  \brief Attaches a child entity to parent
*/
/******************************************************************************/
	void RemoveChild(const EntityID& id);

/******************************************************************************/
/*!
  \fn ~ParentChild()

  \brief ParentChild's destructor to remove component from component manager
*/
/******************************************************************************/
	~ParentChild();
};

#endif