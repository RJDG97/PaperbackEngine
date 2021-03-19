/**********************************************************************************
*\file         TitleCard_Script.h
*\brief        Contains definition of functions and variables used for
*			   the TitleCard Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _TITLECARD_SCRIPT_H_
#define _TITLECARD_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Components/LogicComponent.h"
#include "Systems/GraphicsSystem.h"

namespace TitleCard_Script
{
	void TitleCardAnim(const EntityID& title_card_id) {

		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();
		std::shared_ptr<GraphicsSystem> graphics_sys = CORE->GetSystem<GraphicsSystem>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(title_card_id);

		if (animation_renderer->FinishedAnimating()) {

			std::string comp = animation_renderer->GetCurrentAnimation();

			if (comp == "TitleCard") {

				graphics_sys->ChangeAnimation(animation_renderer, "TitleCard_2");
			}
			else if (comp == "TitleCard_2") {

				graphics_sys->ChangeAnimation(animation_renderer, "TitleCard");
			}
		}
	}

	void TitleTextAnim(const EntityID& title_card_id) {

		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();
		std::shared_ptr<GraphicsSystem> graphics_sys = CORE->GetSystem<GraphicsSystem>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(title_card_id);

		if (!animation_renderer->IsAlive())
			return;

		if (animation_renderer->FinishedAnimating()) {

			std::string comp = animation_renderer->GetCurrentAnimation();

			if (comp == "TitleText_2") {

				animation_renderer->SetAnimationStatus(false);
			}
		}

		float opacity = animation_renderer->GetOpacity();
		float scaling = 1 - opacity;
		float new_opacity = std::min( 1.0f, opacity + (scaling * scaling * 0.08f) + 0.01f);

		animation_renderer->SetOpacity(new_opacity);
	}
}

#endif