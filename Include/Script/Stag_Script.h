#pragma once
#ifndef _ENEMY_SCRIPT_H_
#define _ENEMY_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Manager/ComponentManager.h"
#include "Components/LogicComponent.h"

namespace Stag_Script
{
	void TextureUpdateScript(const EntityID& parent_id) {
		
		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		AnimationRenderer* renderer = component_mgr->GetComponent<AnimationRenderer>(parent_id);
		Motion* motion = component_mgr->GetComponent<Motion>(parent_id);
		Name* name = component_mgr->GetComponent<Name>(parent_id);
		AI* ai = component_mgr->GetComponent<AI>(parent_id);

		// If any pointers are invalid, return
		if (!renderer || !ai || !motion || !name)
			return;

		// If velocity is essentially 0, set player to idle
		if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y)) {

			graphics->ChangeAnimation(renderer, "Stagbeetle_Idle");
		}
		// If velocity isn't 0 set to others
		else {

			switch (ai->GetState())
			{
				case AI::AIState::Return:
				{
					MessageBGM_Play msg{ "EnemyLostSight" };
					CORE->BroadcastMessage(&msg);
					graphics->ChangeAnimation(renderer, "Stagbeetle_Confused");
					break;
				}
				case AI::AIState::Patrol:
				{
					graphics->ChangeAnimation(renderer, "Stagbeetle_Walk");
					break;
				}
				case AI::AIState::Attack:
				{
					MessageBGM_Play msg{ "EnemyAttack" };
					CORE->BroadcastMessage(&msg);
					graphics->ChangeAnimation(renderer, "Stagbeetle_Attack");
					break;
				}
				case AI::AIState::Detected:
				{
					MessageBGM_Play msg{ "EnemyDetect" };
					CORE->BroadcastMessage(&msg);
					graphics->ChangeAnimation(renderer, "Stagbeetle_Alert");
					break;
				}
			}
		}

		if (motion->GetVelocity().x > 0 && motion->GetIsLeft()) {

			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->GetIsLeft()) {

			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
	}
}

#endif