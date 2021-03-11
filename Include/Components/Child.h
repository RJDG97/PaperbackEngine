/**********************************************************************************
*\file         Child.h
*\brief        Contains declaration of functions and variables used for
*			   the Child Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _CHILD_H_
#define _CHILD_H_

#include <vector>
#include <string>
#include "Components/IComponent.h"

class Child : public Component
{
private:
	std::string parent_name_;
	EntityID parent_id_; // Used to de-allocate assigned pointer data when destroying the child
	// EntityID parent_id_; // By right it should be parent id but when we create the "child"
							// in a fresh scene, the ids will be jumbled up so we have to restart
							// the scene potentially before assigning the id which is very complex.
							// Similarly when we delete entities, the ID will have to be updated for
							// for all parents within the scene.

public:

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initialize the component
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
	  \fn DeSerializeClone()

	  \brief Loads a component from JSON format
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
	  \fn GetParentName()

	  \brief Return the name of the Parent
	*/
	/******************************************************************************/
	std::string ParentName() const;

	/******************************************************************************/
	/*!
	  \fn SetParentID()

	  \brief Assigns the parent's id to the child
	*/
	/******************************************************************************/
	void SetParentID(const EntityID& p_id);

	/******************************************************************************/
	/*!
	  \fn ParentID()

	  \brief Retrieves the parent's id
	*/
	/******************************************************************************/
	EntityID ParentID() const;

	/******************************************************************************/
	/*!
	  \fn ~Child()

	  \brief Child's destructor to remove component from component manager
	*/
	/******************************************************************************/
	~Child();
};

#endif