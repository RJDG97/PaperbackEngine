/**********************************************************************************
*\file         TransitionSystem.h
*\brief        Contains definition of functions and variables used for
*			   the Transition System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _TRANSITION_SYSTEM_H_
#define _TRANSITION_SYSTEM_H_

#include "Systems/ISystem.h"
#include "Manager/ComponentManager.h"
#include "Manager/TransitionManager.h"


class TransitionSystem : public ISystem
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
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	std::string GetName() { return "TransitionSystem"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Handles incoming messages and sorts based on message id
*/
/******************************************************************************/
	void SendMessageD(Message* m);

private:

	std::shared_ptr<ComponentManager> component_manager_;
	std::shared_ptr<TransitionManager> transition_manager_;
	std::shared_ptr<GraphicsSystem> graphics_system_;
};


#endif