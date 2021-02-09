/**********************************************************************************
*\file         Collectible_Script.h
*\brief        Contains definition of functions and variables used for
*			   the Collectible Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _COLLECTIBLE_SCRIPT_H_
#define _COLLECTIBLE_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Components/LogicComponent.h"

namespace Collectible_Script
{
	/******************************************************************************/
	/*!
	  \fn CollectedCollectible()

	  \brief Collectible update script for a Puddle / Key
	*/
	/******************************************************************************/
	void CollectedCollectible(const EntityID& collectible_id) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(collectible_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(collectible_id);
		Collectible* collectible = component_mgr->GetComponent<Collectible>(collectible_id);
		PointLight* point_light = component_mgr->GetComponent<PointLight>(collectible_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(collectible_id);

		// Toggle to inactive (Potentially delete them)
		if (animation_renderer)
			animation_renderer->SetAlive(false);
		if (texture_renderer)
			texture_renderer->SetAlive(false);
		if (point_light)
			point_light->SetAlive(false);
		if (aabb)
			aabb->SetAlive(false);

		switch (collectible->GetItemType())
		{
			case CollectibleType::SPORE:
			{
				break;
			}
			case CollectibleType::PUDDLE:
			{
				MessageBGM_Play msg{ "PlayerDrink" };
				CORE->BroadcastMessage(&msg);
				break;
			}
			case CollectibleType::KEY:
			{
				CORE->GetSystem<DialogueSystem>()->SetCurrentDialogue("keypickedup");
				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn InteractableResponse()

	  \brief Interactable update script for Logs
	*/
	/******************************************************************************/
	void InteractableResponse(const EntityID& interactable_id) {

		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();
		std::shared_ptr<GraphicsSystem> graphics_sys = CORE->GetSystem<GraphicsSystem>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(interactable_id);
		Interactable* interactable = component_mgr->GetComponent<Interactable>(interactable_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(interactable_id);

		// By right, switch case based on "enum" for animation changes
		//std::string col_name = interactable->GetAnimationName("Collided");
		//graphics_sys->ChangeAnimation(animation_renderer, col_name);

		animation_renderer->SetAnimationStatus(true);

		if (animation_renderer->FinishedAnimating()) {

			if (aabb) {

				aabb->SetAlive(false);
				animation_renderer->SetAnimationStatus(false);
				//std::string def_name = interactable->GetAnimationName("Default");
				//graphics_sys->ChangeAnimation(animation_renderer, def_name);
			}
		}
	}
}

#endif