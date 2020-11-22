#include "Script/StagScript.h"
#include "Engine/Core.h"
#include "Manager/ForcesManager.h"
#include "Systems/GraphicsSystem.h"

namespace StagBeetle
{
	bool Attack(AIIt obj)
	{
		//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Stagbeetle_Attack");
		// Find current distance of player from obj
		float distance = Vector2DDistance(obj->second->GetPlayerLastPos(), GeneralScripts::obj_rigidbody->GetPosition());
		// If obj is close enough, return true
		if (distance < 0.1f)
		{
			// Fall animation
			return false;
		}
		//get directional unit vector
		Vector2D directional = obj->second->GetPlayerLastPos() - GeneralScripts::obj_rigidbody->GetPosition();
		directional /= Vector2DLength(directional * 2.0f);

		//multiply by speed
		directional *= (obj->second->GetSpeed()*3.0f);

		// Move AI
		GeneralScripts::forces_->AddForce(obj->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);

		return true;
	}

	void Handler(AIIt obj)
	{
		//obj->second->GetTimer().TimerUpdate();
		switch (obj->second->GetState())
		{
		case AI::AIState::Patrol:
			//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Stagbeetle_Walk");
			GeneralScripts::Patrol(obj);
			// Animation (maybe check vector direction to determine which 
			//			  side enemy is facing)
			if (GeneralScripts::DetectPlayer(obj))
				obj->second->SetState(AI::AIState::Detected);
			break;
		case AI::AIState::Detected:
			//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Stagbeetle_Alert");
			obj->second->GetTimer().TimerStart();
			// Player Detected anim
			if (obj->second->GetTimer().TimeElapsed(s) > 1.0f)
			{
				obj->second->GetTimer().TimerStop();
				obj->second->GetTimer().TimerReset();
				obj->second->SetState(AI::AIState::Attack);
			}
			break;
		case AI::AIState::Attack:
			if (!Attack(obj))
				obj->second->SetState(AI::AIState::Return);
			break;
		case AI::AIState::Return:
			//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Stagbeetle_Confused");
			obj->second->GetTimer().TimerStart();
			// Player Detected anim
			if (obj->second->GetTimer().TimeElapsed(s) > 1.0f)
			{
				obj->second->GetTimer().TimerStop();
				obj->second->GetTimer().TimerReset();
				if (GeneralScripts::DetectPlayer(obj)) // Player in radius
				{
					obj->second->SetState(AI::AIState::Attack);
				} // Pursue player
				else
				{
					GeneralScripts::map_->Pathing(obj->second->GetPath(), GeneralScripts::obj_rigidbody->GetOffsetAABBPos(), *obj->second->GetCurrentDes());
					obj->second->SetState(AI::AIState::Patrol);
				}
			}
			break;
		}
	}

}