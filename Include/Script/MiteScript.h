/**********************************************************************************
*\file         MiteScript.h
*\brief        Contains declaration of functions and variables used for
*			   the Mite AI
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _MITESCRIPT_H_
#define _MITESCRIPT_H_

#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"
#include "Components/TextureRenderer.h"

namespace Mite
{
	/******************************************************************************/
	/*!
	  \fn Attack(AIIt obj)

	  \brief Run Mite Attack Pattern
	*/
	/******************************************************************************/
	void Attack(AIIt obj);

	/******************************************************************************/
	/*!
	  \fn Handler(AIIt obj)

	  \brief Handle Mite Logic
	*/
	/******************************************************************************/
	void Handler(AIIt obj);

}

#endif
