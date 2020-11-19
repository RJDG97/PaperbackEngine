#include "Script/MiteScript.h"
#include "Engine/Core.h"
#include "Manager/ForcesManager.h"
#include "Systems/GraphicsSystem.h"
#include "Manager/EntityManager.h"
#include "Systems/Factory.h"

namespace Mite
{
	void Attack(AIIt obj)
	{
		obj->second->GetTimer().TimerStart();
		if (obj->second->GetTimer().TimeElapsed(s) > 0.7f)
		{
			obj->second->GetTimer().TimerStop();
			obj->second->GetTimer().TimerReset();

			float distance = Vector2DDistance(GeneralScripts::player_rigidbody->GetPosition(), GeneralScripts::obj_rigidbody->GetPosition());
			// Explsion animation
			if (distance < 50.0f && !(GeneralScripts::player_status->GetStatus() == StatusType::BURROW ||
				GeneralScripts::player_status->GetStatus() == StatusType::HIT)) // replace with attack radius
			{
				GeneralScripts::player_health->SetCurrentHealth(GeneralScripts::player_health->GetCurrentHealth() - 1);
			}
			CORE->GetSystem<EntityFactory>()->Destroy
			(CORE->GetManager<EntityManager>()->GetEntity(obj->first));
		}
	}

	void Handler(AIIt obj)
	{
		switch (obj->second->GetState())
		{
		case AI::AIState::Patrol:
			if(obj->second->GetNumDes()==1)
				CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Idle");
			else
			{
				CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Walk");
				GeneralScripts::Patrol(obj);
			}
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
			Attack(obj);
			break;
		}
	}

}