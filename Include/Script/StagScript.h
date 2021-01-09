/**********************************************************************************
*\file         StagScript.h
*\brief        Contains declarations of functions and variables used for
*			   the Stag AI
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _STAGSCRIPT_H_
#define _STAGSCRIPT_H_

#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"

namespace StagBeetle
{

	/******************************************************************************/
	/*!
	  \fn Attack(
	  obj)

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