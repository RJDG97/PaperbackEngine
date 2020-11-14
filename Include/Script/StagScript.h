#ifndef _STAGSCRIPT_H_
#define _STAGSCRIPT_H_

#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"

namespace StagBeetle
{
	/******************************************************************************/
	/*!
	  \fn Attack(AIIt obj)

	  \brief Run Stag Beetle Attack Pattern
	*/
	/******************************************************************************/
	bool Attack(AIIt obj);

	/******************************************************************************/
	/*!
	  \fn Handler(AIIt obj)

	  \brief Handle Stag Beetle Logic
	*/
	/******************************************************************************/
	void Handler(AIIt obj);

}

#endif