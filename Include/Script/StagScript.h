#ifndef _STAGSCRIPT_H_
#define _STAGSCRIPT_H_

#include "Manager/AIManager.h"
#include "Script/StagScript.h"
#include "Script/GeneralScripts.h"

class StagBeetle : public AIManager
{
public:

	void Attack(AIIt ai)
	{
		// Get direction from player
		ai->second->speed_ += 5.0f;// Increase speed
		// continue until collision
		// Move AI
		//CORE->GetManager<ForcesManager>()->AddForce(ai->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
		ai->second->speed_ -= 5.0f;// Decrease speed
	}

	void StagBeetleHandler(AIIt ai)
	{
		switch (ai->second->GetState())
		{
		case AI::AIState::Patrol:
			GS.Patrol(ai);
			// Animation (maybe check vector direction to determine which 
			//			  side enemy is facing)
			if (GS.DetectPlayer(ai))
				ai->second->SetState(AI::AIState::Detected);
			break;
		case AI::AIState::Detected:
			// Player Detected anim
			break;
		case AI::AIState::Chase:
			if (!GS.DetectPlayer(ai)) // Player not in radius
			{
				ai->second->SetState(AI::AIState::Return);
			}
			else if (GS.Chase(ai))
			{
				ai->second->SetState(AI::AIState::Attack);
			}
			break;
		case AI::AIState::Attack:
			Attack(ai);
			if (GS.DetectPlayer(ai)) // Player in radius
			{
				ai->second->SetState(AI::AIState::Chase);
			} // Pursue player
			else
			{
				ai->second->SetState(AI::AIState::Return);
			}
			break;
		case AI::AIState::Return:
			break;
		}
	}

}StagBeetle;

#endif