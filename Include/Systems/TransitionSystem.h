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