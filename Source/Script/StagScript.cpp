#include "Script/StagScript.h"

namespace StagBeetle
{
	bool Attack(AIIt ai)
	{
		// Get direction from player
		//ai->second->speed_ += 5.0f;// Increase speed
		// continue until collision
		// Move AI
		//CORE->GetManager<ForcesManager>()->AddForce(ai->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
		//ai->second->speed_ -= 5.0f;// Decrease speed
		return false;
	}

	void Handler(AIIt ai)
	{
		switch (ai->second->GetState())
		{
		case AI::AIState::Patrol:
			GeneralScripts::Patrol(ai);
			// Animation (maybe check vector direction to determine which 
			//			  side enemy is facing)
			if (GeneralScripts::DetectPlayer(ai))
				ai->second->SetState(AI::AIState::Detected);
			break;
		case AI::AIState::Detected:
			// Player Detected anim
			ai->second->SetState(AI::AIState::Chase);
			break;
		case AI::AIState::Chase:
			if (!GeneralScripts::DetectPlayer(ai)) // Player not in radius
			{
				ai->second->SetState(AI::AIState::Patrol);
			}
			else if (GeneralScripts::Chase(ai))
			{
				ai->second->SetState(AI::AIState::Attack);
			}
			break;
		case AI::AIState::Attack:
			if (!Attack(ai))
			{
				if (GeneralScripts::DetectPlayer(ai)) // Player in radius
				{
					ai->second->SetState(AI::AIState::Chase);
				} // Pursue player
				else
				{
					ai->second->SetState(AI::AIState::Patrol);
				}
			}
			break;
		}
	}

}