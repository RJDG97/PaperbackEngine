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
		if (obj->second->GetTimer().TimeElapsed(s) > 0.635f)
		{
			obj->second->GetTimer().TimerStop();
			obj->second->GetTimer().TimerReset();

			float distance = Vector2DDistance(GeneralScripts::player_rigidbody->GetOffsetAABBPos(), GeneralScripts::obj_rigidbody->GetOffsetAABBPos());
			// Explsion animation
			if (distance < 50.0f && !(GeneralScripts::player_status->GetStatus() == StatusType::BURROW ||
				GeneralScripts::player_status->GetStatus() == StatusType::HIT)) // replace with attack radius
			{
				GeneralScripts::player_health->SetCurrentHealth(GeneralScripts::player_health->GetCurrentHealth() - 1);
			}
			GeneralScripts::obj_rigidbody->SetPosition
			(*obj->second->GetDestinations().begin());
			GeneralScripts::comp_mgr->GetComponent<AnimationRenderer>(obj->first)->SetAlive(false);
			obj->second->SetLife(false);
			// CORE->GetManager<ComponentManager>()->GetComponent<TextureRenderer>(obj->first)->SetLife(false);
		}
	}

	void Handler(AIIt obj)
	{

		if (obj->second->GetLife()) {
			switch (obj->second->GetState())
			{
			case AI::AIState::Patrol:
				if (obj->second->GetNumDes() == 1)
					CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Idle");
				else
				{
					//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Walk");
					GeneralScripts::Patrol(obj);
				}
				if (GeneralScripts::DetectPlayer(obj))
					GeneralScripts::map_->Pathing(obj->second->GetPath(), 
						GeneralScripts::obj_rigidbody->GetOffsetAABBPos(), 
						GeneralScripts::player_rigidbody->GetOffsetAABBPos());
					obj->second->SetState(AI::AIState::Chase);
				break;
			case AI::AIState::Detected:
				// Player Detected anim
				//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Alert");
				obj->second->GetTimer().TimerStart();
				if (obj->second->GetTimer().TimeElapsed(s) > 0.1f)
				{
					obj->second->GetTimer().TimerStop();
					obj->second->GetTimer().TimerReset();
					obj->second->SetState(AI::AIState::Chase);
				}
				break;
			case AI::AIState::Withdraw:
				GeneralScripts::map_->Pathing(obj->second->GetPath(), GeneralScripts::obj_rigidbody->GetOffsetAABBPos(), GeneralScripts::player_rigidbody->GetOffsetAABBPos());
				obj->second->SetState(AI::AIState::Chase);
				break;
			case AI::AIState::Attack:
				//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Explode");
				Attack(obj);
				break;
			case AI::AIState::Chase:
				//CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(GeneralScripts::obj_anim_renderer, "Mite_Walk");
				float distance = Vector2DDistance(GeneralScripts::obj_rigidbody->GetOffsetAABBPos(), GeneralScripts::player_rigidbody->GetOffsetAABBPos());
				if (distance > 5.0f) // Player not in radius
				{
					obj->second->SetState(AI::AIState::Patrol);
				}
				else if (GeneralScripts::Chase(obj))
				{
					obj->second->SetState(AI::AIState::Attack);
				}
				break;
			}
		}
		else {
			obj->second->GetTimer().TimerStart();
			if (obj->second->GetTimer().TimeElapsed(s) > 5.0f)
			{
				obj->second->GetTimer().TimerStop();
				obj->second->GetTimer().TimerReset();
				obj->second->SetState(AI::AIState::Patrol);
				obj->second->GetPath().clear();
				GeneralScripts::comp_mgr->GetComponent<AnimationRenderer>(obj->first)->SetAlive(true);
				obj->second->SetLife(true);
				//CORE->GetManager<ComponentManager>()->GetComponent<TextureRenderer>(obj->first)->SetLife(true);
			}
		}
	}

};