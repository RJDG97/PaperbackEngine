#pragma once
#ifndef _MITE_SCRIPT_H_
#define _MITE_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Manager/ComponentManager.h"
#include "Components/LogicComponent.h"

namespace Mite_Script
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

			graphics->ChangeAnimation(renderer, "Mite_Idle");
		}
		// If velocity isn't 0 set to others
		else {

			switch (ai->GetState())
			{
				case AI::AIState::Return:
				case AI::AIState::Chase:
				case AI::AIState::Patrol:
				{
					graphics->ChangeAnimation(renderer, "Mite_Walk");
					break;
				}
				case AI::AIState::Attack:
				{
					graphics->ChangeAnimation(renderer, "Mite_Explode");
					break;
				}
				case AI::AIState::Detected:
				{
					graphics->ChangeAnimation(renderer, "Mite_Alert");
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