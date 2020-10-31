#ifndef _HORNETSCRIPT_H_
#define _HORNETSCRIPT_H_

#include "Manager/AIManager.h"
#include "Script/HornetScript.h"
#include "Script/GeneralScripts.h"

class Hornet : public AIManager
{
public:

	void HornetHandler(AIIt ai)
	{
		switch (ai->second->GetState())
		{
		case AI::AIState::Patrol:
			Patrol(ai);
			break;
		}
	}

	void StingAttack(AIIt ai)
	{

	}

}hornet;

#endif
