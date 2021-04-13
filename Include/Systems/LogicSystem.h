/**********************************************************************************
*\file         LogicSystem.h
*\brief        Contains declaration of functions and variables used for the LogicSystem
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _LOGICSYSTEM_H_
#define _LOGICSYSTEM_H_

#include <iostream>
#include <unordered_map>
#include "Systems/Message.h"
#include "Components/AI.h"
#include "Systems/ISystem.h"
#include "Manager/ComponentManager.h"
#include "Components/ParentChild.h"

using AIIt = std::unordered_map<EntityID, AI*>::iterator;
using AIType = CMap<AI>;
using AIIt = AIType::MapTypeIt;

class LogicSystem: public ISystem{

	//std::unordered_map<EntityID, AI*> ai_arr_;
	AIType* ai_arr_;
	ComponentManager* comp_mgr;

public:

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initializes
	*/
	/******************************************************************************/
	virtual void Init() override;

	/******************************************************************************/
	/*!
	  \fn Update()

	  \brief Performs update
	*/
	/******************************************************************************/
	virtual void Update(float frametime) override;

	/******************************************************************************/
	/*!
	  \fn Draw()

	  \brief Used to draw
	*/
	/******************************************************************************/
	virtual void Draw() override;

	/******************************************************************************/
	/*!
	  \fn GetName()

	  \brief Returns the name of the system
	*/
	/******************************************************************************/
	virtual std::string GetName() override { return "LogicSystem"; }

	/******************************************************************************/
	/*!
	  \fn SendMessageD()

	  \brief Receives messages broadcasted from Core Engine and processes it
	*/
	/******************************************************************************/
	virtual void SendMessageD(Message* m) override;

	/******************************************************************************/
	/*!
	  \fn ~LogicSystem()

	  \brief Default implementation destructor
	*/
	/******************************************************************************/
	virtual ~LogicSystem() = default;
	
};

#endif