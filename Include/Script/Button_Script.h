/**********************************************************************************
*\file         Mite_Script.h
*\brief        Contains definition of functions and variables used for
*			   the Mite Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _BUTTON_SCRIPT_H_
#define _BUTTON_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Components/LogicComponent.h"

namespace Button_Script
{
	void BurrowUI_TextureUpdateScript(const EntityID& parent_id) {

		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		EntityManager* entity_mgr = &*CORE->GetManager<EntityManager>();
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		TextureRenderer* renderer = component_mgr->GetComponent<TextureRenderer>(parent_id);
		std::vector<Entity*> player_arr = entity_mgr->GetPlayerEntities();

		// If any pointers are invalid, return
		if (!renderer || !player_arr.size())
			return;

		Status* player_status = component_mgr->GetComponent<Status>(player_arr.back()->GetID());

		// If player's status component does not exist, return
		if (!player_status)
			return;

		if (player_status->GetStatus() == StatusType::BURROW) {
			
			graphics->ChangeTexture(renderer, "BurrowSkill_UITint_0");
		}
		else {

			graphics->ChangeTexture(renderer, "BurrowSkill_UI_0");
		}
	}


	void HideUI_TextureUpdateScript(const EntityID& parent_id) {

		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		EntityManager* entity_mgr = &*CORE->GetManager<EntityManager>();
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		TextureRenderer* renderer = component_mgr->GetComponent<TextureRenderer>(parent_id);
		std::vector<Entity*> player_arr = entity_mgr->GetPlayerEntities();

		// If any pointers are invalid, return
		if (!renderer || !player_arr.size())
			return;

		Status* player_status = component_mgr->GetComponent<Status>(player_arr.back()->GetID());

		// If player's status component does not exist, return
		if (!player_status)
			return;

		if (player_status->GetStatus() == StatusType::INVISIBLE) {

			graphics->ChangeTexture(renderer, "HideSkill_UITint_0");
		}
		else {

			graphics->ChangeTexture(renderer, "HideSkill_UI_0");
		}
	}
}

#endif