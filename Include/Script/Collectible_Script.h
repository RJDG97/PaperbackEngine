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
		//Transform* xform = component_mgr->GetComponent<Transform>(collectible_id);
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

				//ParentChild* pc = component_mgr->GetComponent<ParentChild>(collectible_id);
				//if (pc) {

				//	// Grab the children (Emitter)
				//	std::vector<Entity*> child = pc->GetChildren();

				//	if (child.empty())
				//		return;

				//	EntityID emitter_id = child.back()->GetID();
				//	Emitter* emitter = component_mgr->GetComponent<Emitter>(emitter_id);
				//	Vector2D pos = xform->GetOffsetAABBPos();

				//	emitter->SetAlive(true);
				//	emitter->SetLifeTime(2.0f);
				//	emitter->SetPositionStruct(pos, pos);
				//	emitter->SetForceStruct({ 10.0f, 30.0f }, { 60.0f, -60.0f });
				//	emitter->SetLifeTimeStruct({ 1.7f, 1.7f });
				//	emitter->SetMaxNumberParticles(5);
				//	emitter->SetRequest(5);
				//	emitter->SetRotationStruct({ 1.0f, 2.0f }, {}, {});
				//	emitter->SetSpawnInterval(1.7f);
				//	emitter->SetTextureStruct(1, {"Puddle_0"});
				//	emitter->SetDestinationStruct({ -8.7f, 3.7f }, { 2.0f, 4.0f }, true);
				//}

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
		std::shared_ptr<SoundSystem> sound_sys = CORE->GetSystem<SoundSystem>();

		// Grab relevant components
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(interactable_id);
		//Interactable* interactable = component_mgr->GetComponent<Interactable>(interactable_id);
		AABB* aabb = component_mgr->GetComponent<AABB>(interactable_id);

		// By right, switch case based on "enum" for animation changes
		//std::string col_name = interactable->GetAnimationName("Collided");
		//graphics_sys->ChangeAnimation(animation_renderer, col_name);

		animation_renderer->SetAnimationStatus(true);

		sound_sys->PlaySounds("PlayerPushTree");

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