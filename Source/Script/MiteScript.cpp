#include "Script/MiteScript.h"
#include "Engine/Core.h"
#include "Manager/ForcesManager.h"

namespace Mite
{
	bool Attack(AIIt obj)
	{
		// Find current distance of player from obj
		float distance = Vector2DDistance(GeneralScripts::player_rigidbody->GetPosition(), GeneralScripts::obj_rigidbody->GetPosition());
		// Explsion animation
		if (distance < 50.0f) // replace with attack radius
		{
			// decrement player health
			return true;
		}
		return true;
	}

	void Handler(AIIt obj)
	{
		switch (obj->second->GetState())
		{
		case AI::AIState::Patrol:
			GeneralScripts::Patrol(obj);
			// Animation (maybe check vector direction to determine which 
			//			  side enemy is facing)
			if (GeneralScripts::DetectPlayer(obj))
				obj->second->SetState(AI::AIState::Detected);
			break;
		case AI::AIState::Detected:
			// Player Detected anim
			obj->second->SetState(AI::AIState::Chase);
			break;
		case AI::AIState::Chase:
			if (!GeneralScripts::DetectPlayer(obj)) // Player not in radius
			{
				obj->second->SetState(AI::AIState::Patrol);
			}
			else if (GeneralScripts::Chase(obj))
			{
				obj->second->SetState(AI::AIState::Attack);
			}
			break;
		case AI::AIState::Attack:
			if (!Attack(obj))
			{
				// Destroy entity
				// Or respawn it?
			}
			break;
		}
	}

}