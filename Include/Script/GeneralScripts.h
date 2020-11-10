#ifndef _GENERALSCRIPT_H_
#define _GENERALSCRIPT_H_

#include "Systems/LogicSystem.h"
#include "Script/ScriptList.h"
#include "Manager/AMap.h"

namespace GeneralScripts
{
	extern EntityID player_id;
	extern Transform* player_rigidbody;
	extern Transform* obj_rigidbody;
	extern AMap* map;

	//AI* obj_{nullptr};
	bool Chase(AIIt obj);

	bool DetectPlayer(AIIt obj);
	
	void AIHandler(AIIt obj);

	AI::AIType GetType(std::string type);

	void Patrol(AIIt obj);
}

#endif