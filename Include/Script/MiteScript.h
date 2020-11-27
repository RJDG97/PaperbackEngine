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
