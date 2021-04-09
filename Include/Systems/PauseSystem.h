/**********************************************************************************
*\file         PauseSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Pause System
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _PAUSE_SYSTEM_H_
#define _PAUSE_SYSTEM_H_

#include <map>
#include "Manager/ComponentManager.h"
#include "Manager/LogicManager.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/ISystem.h"

class PauseSystem : public ISystem
{
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
	  \fn SetActiveLayer()

	  \brief Used to toggle the current "Button" layer to active
	*/
	/******************************************************************************/
	void SetActiveLayer(const int& layer);

	/******************************************************************************/
	/*!
	  \fn EnableNextLayer()

	  \brief Enable the next pause menu layer
	*/
	/******************************************************************************/
	void EnableNextLayer();

	/******************************************************************************/
	/*!
	  \fn RevertPreviousLayer()

	  \brief Revert to the previous pause menu layer, disables previous layer if
			 it is the first layer
	*/
	/******************************************************************************/
	bool RevertPreviousLayer();

	/******************************************************************************/
	/*!
	  \fn InitializeClickables()

	  \brief Initializes each level's clickables
	*/
	/******************************************************************************/
	void InitializeClickables();

	/******************************************************************************/
	/*!
	  \fn ClearSystem()

	  \brief Clear the current level's clickables
	*/
	/******************************************************************************/
	void ClearSystem();

	/******************************************************************************/
	/*!
	  \fn PrevLayer()

	  \brief Retrieves the previous layer value
	*/
	/******************************************************************************/
	const int& PrevLayer() const;

	/******************************************************************************/
	/*!
	  \fn TerminateState()

	  \brief Set to true to pause controls indefinitely
	*/
	/******************************************************************************/
	void TerminateState(bool status);

	/******************************************************************************/
	/*!
	  \fn GetState()

	  \brief Retrieves the state ended status
	*/
	/******************************************************************************/
	bool GetState() const;

	/******************************************************************************/
	/*!
	  \fn GetName()

	  \brief Returns the name of the system
	*/
	/******************************************************************************/
	std::string GetName() { return "SettingsSystem"; }

	/******************************************************************************/
	/*!
	  \fn SendMessageD()

	  \brief Handles incoming messages and sorts based on message id
	*/
	/******************************************************************************/
	void SendMessageD(Message* m) {
		(void)m;
	}


private:

	using Group = int;
	using OrderInGroup = int;

	using ClickableMap = CMap<Clickable>;
	using ClickableIt = ClickableMap::MapTypeIt;

	using PauseMap = std::map< Group, std::map< OrderInGroup, std::vector<EntityID> > >;
	//using PauseMap = std::map< int, std::vector<EntityID> >;

	std::shared_ptr<ComponentManager> mgr;
	std::shared_ptr<LogicManager> logic_mgr;
	std::shared_ptr<GraphicsSystem> graphics;
	ClickableMap* click_map_;
	PauseMap pause_layering_;
	bool ended_;

	// Previous active_layer_
	int previous_layer_;

	// If active_layer_ == -1, do nothing
	// If active_layer_ != -1, set entities on that previous_layer_ to NotAlive
	// then set active_layer_'s entities to Alive, and set previous_layer_ to 
	// active_layer
	int active_layer_;

	size_t order_;
};


#endif