#ifndef _MITESCRIPT_H_
#define _MITESCRIPT_H_

#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"

namespace Mite
{
	/******************************************************************************/
	/*!
	  \fn Attack(AIIt obj)

	  \brief Run Mite Attack Pattern
	*/
	/******************************************************************************/
	bool Attack(AIIt obj);

	/******************************************************************************/
	/*!
	  \fn Handler(AIIt obj)

	  \brief Handle Mite Logic
	*/
	/******************************************************************************/
	void Handler(AIIt obj);

}

#endif
