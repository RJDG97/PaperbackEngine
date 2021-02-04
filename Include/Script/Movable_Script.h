#pragma once
#ifndef _MOVABLE_SCRIPT_H_
#define _MOVABLE_SCRIPT_H_

#include <cmath>
#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Components/LogicComponent.h"
#include "Systems/GraphicsSystem.h"

namespace Movable_Script
{
	/******************************************************************************/
	/*!
	  \fn UpdateMovable()

	  \brief Animation update script for the Boulder Entity
	*/
	/******************************************************************************/
	void UpdateMovable(const EntityID& id) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics = &*CORE->GetSystem< GraphicsSystem>();

		AABB* aabb = component_mgr->GetComponent<AABB>(id);
		Motion* motion = component_mgr->GetComponent<Motion>(id);
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(id);

		if (aabb->IsColliding()) {

			if (animation_renderer) {

				if (motion) {

					Vector2D vec = motion->GetVelocity();
					
					if (std::abs(vec.x) > std::abs(vec.y))
						graphics->ChangeAnimation(animation_renderer, "Stone_Roll_Two");
					else
						graphics->ChangeAnimation(animation_renderer, "Stone_Roll_One");

					animation_renderer->SetAnimationStatus(true);

					if (motion->GetVelocity().x > 0 && motion->IsLeft()) {

						graphics->FlipTextureY(animation_renderer);
						motion->SetIsLeft(false);
					}
					else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {

						graphics->FlipTextureY(animation_renderer);
						motion->SetIsLeft(true);
					}

					//if (motion->GetVelocity().y > 0 && !motion->IsTop()) {

					//	graphics->FlipTextureX(animation_renderer);
					//	motion->SetIsTop(true);
					//}
					//else if (motion->GetVelocity().y < 0 && motion->IsTop()) {

					//	graphics->FlipTextureX(animation_renderer);
					//	motion->SetIsTop(false);
					//}
				}
			}
		}
		else {
			// If valid and animation is completed, toggle off
			if (animation_renderer && animation_renderer->FinishedAnimating())
				animation_renderer->SetAnimationStatus(false);
		}
	}
}

#endif