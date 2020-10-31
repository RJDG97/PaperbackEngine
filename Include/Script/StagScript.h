#ifndef _STAGSCRIPT_H_
#define _STAGSCRIPT_H_

#include "Manager/AIManager.h"
#include "Script/StagScript.h"
#include "Script/GeneralScripts.h"

class StagBeetle : public AIManager
{
public:

	void StagBeetleHandler(AIIt ai)
	{
		switch (ai->second->GetState())
		{
		case AI::AIState::Patrol:
			Patrol(ai);
			break;
		case AI::AIState::Attack:
			break;
		}
	}

}StagBeetle;

#endif