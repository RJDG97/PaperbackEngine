/**********************************************************************************
*\file         Parenting.h
*\brief        Contains declaration of functions and variables used for
*			   the Parenting System
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _PARENTING_SYSTEM_H_
#define _PARENTING_SYSTEM_H_

#include "Systems/ISystem.h"
#include "Manager/LogicManager.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"

class ParentingSystem : public ISystem
{
public:
	using ChildMap = CMap<Child>;
	using ChildMapIt = ChildMap::MapTypeIt;

	using ParentMap = CMap<ParentChild>;
	using ParentMapIt = ParentMap::MapTypeIt;


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
	  \fn LinkParentAndChild()

	  \brief Used to link the parent and child entities together at the start of a 
			 scene
	*/
	/******************************************************************************/
	void LinkParentAndChild();

	/******************************************************************************/
	/*!
	  \fn AddChildToParent()

	  \brief Attaches a child to a parent
	*/
	/******************************************************************************/
	void AddChildToParent(const EntityID& parent_id, const EntityID& child_id);

	/******************************************************************************/
	/*!
	  \fn RemoveChildFromParent()

	  \brief Removes a child from a parent
	*/
	/******************************************************************************/
	void RemoveChildFromParent(const EntityID& parent_id, const EntityID& child_id);

	/******************************************************************************/
	/*!
	  \fn UpdateChildOffset()

	  \brief Update child entity offset
	*/
	/******************************************************************************/
	void UpdateChildOffset(const EntityID& parent_id);

	/******************************************************************************/
	/*!
	  \fn GetName()

	  \brief Returns the name of the system
	*/
	/******************************************************************************/
	std::string GetName() { return "ParentingSystem"; }

	/******************************************************************************/
	/*!
	  \fn SendMessageD()

	  \brief Handles incoming messages and sorts based on message id
	*/
	/******************************************************************************/
	void SendMessageD(Message* m);


private:
	std::shared_ptr<LogicManager> logic_manager_;
	std::shared_ptr<EntityManager> entity_manager_;
	std::shared_ptr<ComponentManager> component_manager_;
	ChildMap* child_arr_;
	ParentMap* parent_arr_;
};


#endif