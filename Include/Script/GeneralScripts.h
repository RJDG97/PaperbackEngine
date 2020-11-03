#ifndef _GENERALSCRIPT_H_
#define _GENERALSCRIPT_H_

#include "Systems/LogicSystem.h"

namespace GeneralScripts
{
	//AI* obj_{nullptr};
	
	void AIHandler(AIIt obj);

	AI::AIType GetType(std::string type);

	void Patrol(AIIt ai);
}

#endif