#ifndef _GENERALSCRIPT_H_
#define _GENERALSCRIPT_H_

#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"

namespace GeneralScripts
{
	//AI* obj_{nullptr};
	bool Chase(AIIt obj);

	bool DetectPlayer(AIIt obj);
	
	void AIHandler(AIIt obj);

	AI::AIType GetType(std::string type);

	void Patrol(AIIt ai);
}

#endif