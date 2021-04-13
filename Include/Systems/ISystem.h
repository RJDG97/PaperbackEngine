/**********************************************************************************
*\file         ISystem.h
*\brief        Contains declaration of the interface for Game Systems
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _ISYSTEM_H_
#define _ISYSTEM_H_

#include <string>
#include "Systems/Message.h"

class ISystem {
public:

/******************************************************************************/
/*!
  \fn Init()

  \brief Used to initialise a system if required;
*/
/******************************************************************************/
	virtual void Init() {}

/******************************************************************************/
/*!
  \fn Update()

  \brief Used to run logic that is required every game loop.
*/
/******************************************************************************/
	virtual void Update(float frametime) = 0;

/******************************************************************************/
/*!
  \fn Draw()

  \brief Used to contain logic for rendering activities, to use if necessary.
*/
/******************************************************************************/
	virtual void Draw() {}

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() = 0;

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Handles incoming messages and sorts based on message id
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m) = 0;

/******************************************************************************/
/*!
  \fn ~ISystem()

  \brief Virtual destructor so derived systems can call their destructors
*/
/******************************************************************************/
	virtual ~ISystem() = default;
};

#endif