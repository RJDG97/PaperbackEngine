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
		std::cout << "Hornet" << std::endl;
		switch (ai->second->GetState())
		{
		case AI::AIState::Patrol:
			GS.Patrol(ai);
			break;
		}
	}

	void StingAttack(AIIt ai)
	{

	}

}hornet;

#endif
