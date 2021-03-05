/**********************************************************************************
*\file         UI_Script.h
*\brief        Contains definition of functions and variables used for
*			   the UI Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _UI_SCRIPT_H_
#define _UI_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Components/LogicComponent.h"

namespace UI_Script
{
	/******************************************************************************/
	/*!
	  \fn BurrowUI_TextureUpdateScript()

	  \brief Texture update script for Burrow UI Button
	*/
	/******************************************************************************/
	void BurrowUI_TextureUpdateScript(const EntityID& parent_id) {

		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		EntityManager* entity_mgr = &*CORE->GetManager<EntityManager>();
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		TextureRenderer* renderer = component_mgr->GetComponent<TextureRenderer>(parent_id);
		Entity* player_ = entity_mgr->GetPlayerEntities();

		// If any pointers are invalid, return
		if (!renderer || !player_)
			return;

		Status* player_status = component_mgr->GetComponent<Status>(player_->GetID());

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


	/******************************************************************************/
	/*!
	  \fn HideUI_TextureUpdateScript()

	  \brief Texture update script for Hide UI Button
	*/
	/******************************************************************************/
	void HideUI_TextureUpdateScript(const EntityID& parent_id) {

		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		EntityManager* entity_mgr = &*CORE->GetManager<EntityManager>();
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		TextureRenderer* renderer = component_mgr->GetComponent<TextureRenderer>(parent_id);
		Entity* player_ = entity_mgr->GetPlayerEntities();

		// If any pointers are invalid, return
		if (!renderer || !player_)
			return;

		Status* player_status = component_mgr->GetComponent<Status>(player_->GetID());

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