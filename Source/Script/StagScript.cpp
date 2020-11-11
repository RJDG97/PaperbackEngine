#include "Script/StagScript.h"
#include "Engine/Core.h"
#include "Manager/ForcesManager.h"

namespace StagBeetle
{
	bool Attack(AIIt obj)
	{
		// Find current distance of player from obj
		float distance = Vector2DDistance(obj->second->GetPlayerLastPos(), GeneralScripts::obj_rigidbody->GetPosition());
		// If obj is close enough, return true
		if (distance < 0.1f)
		{
			// Fall animation
			// Collision check for damage
			return false;
		}

		//get directional unit vector
		Vector2D directional = obj->second->GetPlayerLastPos() - GeneralScripts::obj_rigidbody->GetPosition();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= (obj->second->GetSpeed());

		// Move AI
		GeneralScripts::forces_->AddForce(obj->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);

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
			obj->second->SetState(AI::AIState::Attack);
			break;
		case AI::AIState::Attack:
			if (!Attack(obj))
			{
				if (GeneralScripts::DetectPlayer(obj)) // Player in radius
				{
					obj->second->SetState(AI::AIState::Attack);
				} // Pursue player
				else
				{
					obj->second->SetState(AI::AIState::Patrol);
				}
			}
			break;
		}
	}

}