/**********************************************************************************
*\file         GeneralScripts.h
*\brief        Contains declaration of functions and variables used for
*			   generic AI in the game
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _GENERALSCRIPT_H_
#define _GENERALSCRIPT_H_

#include "Manager/ForcesManager.h"
#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"
#include "Manager/AMap.h"

namespace GeneralScripts
{
	extern ComponentManager* comp_mgr;

	extern EntityID player_id;
	extern Transform* player_rigidbody;
	extern Transform* obj_rigidbody;
	extern AnimationRenderer* obj_anim_renderer;
	extern Status* player_status;
	extern Health* player_health;
	extern AMap* map_;
	extern ForcesManager* forces_;

	/******************************************************************************/
	/*!
	  \fn Chase(AIIt obj)

	  \brief Object Chase player
	*/
	/******************************************************************************/
	bool Chase(AIIt obj);

	/******************************************************************************/
	/*!
	  \fn DetectPlayer(AIIt obj)

	  \brief Check whether Object can detect player
	*/
	/******************************************************************************/
	bool DetectPlayer(AIIt obj);
	
	/******************************************************************************/
	/*!
	  \fn AIHandler(AIIt obj)

	  \brief Handle AI Logic
	*/
	/******************************************************************************/
	void AIHandler(AIIt obj);

	/******************************************************************************/
	/*!
	  \fn GetType(std::string type)

	  \brief Return AI Types
	*/
	/******************************************************************************/
	AI::AIType GetType(std::string type);

	/******************************************************************************/
	/*!
	  \fn ReturnStringType(const AI::AIType& type)

	  \brief Return AI Type in string form
	*/
	/******************************************************************************/
	std::string ReturnStringType(const AI::AIType& type);

	/******************************************************************************/
	/*!
	  \fn Patrol(AIIt obj)

	  \brief Set Enemy to Patrol waypoints
	*/
	/******************************************************************************/
	void Patrol(AIIt obj);
}

#endif