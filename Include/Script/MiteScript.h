#ifndef _MITESCRIPT_H_
#define _MITESCRIPT_H_

#include "Manager/AIManager.h"
#include "Script/MiteScript.h"
#include "Script/GeneralScripts.h"

class Mite : public AIManager
{
public:

	void MiteHandler(AIIt ai)
	{
		switch (ai->second->GetState())
		{
		case AI::AIState::Patrol:
			Patrol(ai);
			break;
		}
	}

	void ExplosionAttack(AIIt ai)
	{
		std::cout << "Attack" << std::endl;
		ai->second->SetState(AI::AIState::Patrol);
	}

}mite;

#endif
