#include "Script/MiteScript.h"
#include "Engine/Core.h"
#include "Manager/ForcesManager.h"
#include "Systems/GraphicsSystem.h"

namespace Mite
{
	bool Attack(AIIt obj)
	{
		UNREFERENCED_PARAMETER(obj);
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
			CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Idle");
			if (GeneralScripts::DetectPlayer(obj))
				obj->second->SetState(AI::AIState::Detected);
			break;
		case AI::AIState::Detected:
			// Player Detected anim
			CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Alert");
			obj->second->GetTimer().TimerStart();
			if (obj->second->GetTimer().TimeElapsed(s) > 0.5f)
			{
				obj->second->GetTimer().TimerStop();
				obj->second->GetTimer().TimerReset();
				obj->second->SetState(AI::AIState::Chase);
			}
			break;
		case AI::AIState::Chase:
			CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Walk");
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
			CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Explode");
			obj->second->GetTimer().TimerStart();
			if (obj->second->GetTimer().TimeElapsed(s) > 1.0f)
			{
				obj->second->GetTimer().TimerStop();
				obj->second->GetTimer().TimerReset();

				float distance = Vector2DDistance(GeneralScripts::player_rigidbody->GetPosition(), GeneralScripts::obj_rigidbody->GetPosition());
				// Explsion animation
				if (distance < 50.0f) // replace with attack radius
				{
					obj->second->SetState(AI::AIState::Patrol);
				}
				obj->second->SetState(AI::AIState::Patrol);
			}
			break;
		}
	}

}