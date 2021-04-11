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
#include "Systems/EffectsSystem.h"

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
		ForcesManager* forces_mgr = &*CORE->GetManager<ForcesManager>();
		std::shared_ptr<SoundSystem> sound_sys = CORE->GetSystem<SoundSystem>();
		Status* player_status = component_mgr->GetComponent<Status>(CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID());
		if (player_status->GetStatus() == StatusType::BURROW)
			return;

		// Grab relevant components
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(collectible_id);
		Collectible* collectible = component_mgr->GetComponent<Collectible>(collectible_id);
		PointLight* point_light = component_mgr->GetComponent<PointLight>(collectible_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(collectible_id);

		switch (collectible->GetItemType())
		{
			case CollectibleType::SPORE:
			{
				if (aabb) aabb->SetAlive(false);

				Motion* motion = component_mgr->GetComponent<Motion>(collectible_id);
				Transform* xform = component_mgr->GetComponent<Transform>(collectible_id);
				Destination* des = component_mgr->GetComponent<Destination>(collectible_id);

				Vector2D direction = des->GetDestination() - xform->GetOffsetAABBPos();
				Vector2DNormalize(direction, direction);
				sound_sys->PlayTaggedSounds("player_sprout");
				CORE->GetSystem<EffectsSystem>()->spore_size_effect_.SetStatus(0.5f);
				// This dialogue does not exist yet, add in your own version if you are interested wheeeee
				//CORE->GetSystem<DialogueSystem>()->SetCurrentDialogue("Spore_Collected");
				forces_mgr->AddForce(collectible_id, "Collected", 5.0f, direction * motion->GetForce());

				break;
			}
			case CollectibleType::PUDDLE:
			{
				Health* player_hp = component_mgr->GetComponent<Health>(CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID());

				if (player_hp->GetCurrentHealth() >= player_hp->GetMaxHealth())
					return;

				if (texture_renderer) texture_renderer->SetAlive(false);
				if (point_light) point_light->SetAlive(false);
				if (aabb) aabb->SetAlive(false);

				CORE->GetSystem<EffectsSystem>()->health_effect_.SetStatus(0.8f, false);
				sound_sys->PlayTaggedSounds("drink");

				ParentChild* pc = component_mgr->GetComponent<ParentChild>(collectible_id);

				if (!pc->GetChildren().empty()) {
					
					EntityID emitter_id = pc->GetChildren().front()->GetID();
					Emitter* emitter = component_mgr->GetComponent<Emitter>(emitter_id);

					emitter->SetAlive(true);
				}

				break;
			}
			case CollectibleType::KEY:
			{
				if (texture_renderer) texture_renderer->SetAlive(false);
				if (point_light) point_light->SetAlive(false);
				if (aabb) aabb->SetAlive(false);

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
	/*void InteractableResponse(const EntityID& interactable_id) {

		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();

		Status* player_status = component_mgr->GetComponent<Status>(CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID());
		if (player_status->GetStatus() == StatusType::BURROW)
			return;

		std::shared_ptr<GraphicsSystem> graphics_sys = CORE->GetSystem<GraphicsSystem>();
		std::shared_ptr<SoundSystem> sound_sys = CORE->GetSystem<SoundSystem>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(interactable_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(interactable_id);

		animation_renderer->SetAnimationStatus(true);
		CORE->SetMovementLock(true);

		//sound_sys->PlaySounds("PlayerPushTree");
		sound_sys->PlayTaggedSounds("tree_fall");

		if (animation_renderer->FinishedAnimating()) {

			if (aabb) {

				aabb->SetAlive(false);
				animation_renderer->SetAnimationStatus(false);
				CORE->SetMovementLock(false);
			}
		}
	}*/

	/******************************************************************************/
	/*!
	  \fn InteractableResponse()

	  \brief Interactable update script for friend crockpot
	*/
	/******************************************************************************/
	void InteractableResponse(const EntityID& interactable_id) {

		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();

		Status* player_status = component_mgr->GetComponent<Status>(CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID());
		if (player_status->GetStatus() == StatusType::BURROW)
			return;

		std::shared_ptr<GraphicsSystem> graphics_sys = CORE->GetSystem<GraphicsSystem>();
		std::shared_ptr<SoundSystem> sound_sys = CORE->GetSystem<SoundSystem>();
		std::shared_ptr<CameraSystem> camera_sys = CORE->GetSystem<CameraSystem>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(interactable_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(interactable_id);

		if (animation_renderer->GetCurrentAnimation() == "Crockpot_Cooking_t") {

			graphics_sys->ChangeAnimation(animation_renderer, "Crockpot_Fall_t");
			camera_sys->ScreenShake(5.0f, 0.6f, 0.32f);
		}
		CORE->SetMovementLock(true);

		//sound_sys->PlaySounds("PlayerPushTree");
		sound_sys->PlayTaggedSounds("tree_fall");

		if (animation_renderer->FinishedAnimating()) {

			if (aabb) {

				aabb->SetAlive(false);
				animation_renderer->SetAnimationStatus(false);
				CORE->SetMovementLock(false);
			}
		}
	}
}

#endif