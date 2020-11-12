#ifndef _GENERALSCRIPT_H_
#define _GENERALSCRIPT_H_

#include "Manager/ForcesManager.h"
#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"
#include "Manager/AMap.h"

namespace GeneralScripts
{
	extern EntityID player_id;
	extern Transform* player_rigidbody;
	extern Transform* obj_rigidbody;
	extern Status* player_status;
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
	  \fn Patrol(AIIt obj)

	  \brief Set Enemy to Patrol waypoints
	*/
	/******************************************************************************/
	void Patrol(AIIt obj);
}

#endif