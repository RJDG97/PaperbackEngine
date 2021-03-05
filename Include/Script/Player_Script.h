/**********************************************************************************
*\file         Player_Script.h
*\brief        Contains definition of functions and variables used for
*			   the Player Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _PLAYER_SCRIPT_H_
#define _PLAYER_SCRIPT_H_

#include <cstdlib>

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/Physics.h"
#include "Systems/Collision.h"
#include "Systems/DialogueSystem.h"
#include "Systems/Game.h"
#include "Manager/ComponentManager.h"
#include "Components/LogicComponent.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"

void PlayBurrow(Status* status) {
	
	if (status->GetStatus() == StatusType::BURROW) {

		int value = std::rand() % 3;
		std::string sound{"PlayerBurrowing_"};
		sound += std::to_string(value);
		CORE->GetSystem<SoundSystem>()->PlaySounds(sound);
	}
}


namespace Player_Scripts
{
	void CollectedCollectible(const EntityID& player_id, const EntityID& collectible_id) {
		
		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		Collectible* collectible = component_mgr->GetComponent<Collectible>(collectible_id);

		if (!collectible)
			return;
		
		switch (collectible->GetItemType())
		{
			case CollectibleType::PUDDLE:
			{
				Health* health = component_mgr->GetComponent<Health>(player_id);

				if (health) {
					health->IncrementHealth();
				}

				break;
			}
			case CollectibleType::KEY:
			{
				Inventory* inventory = component_mgr->GetComponent<Inventory>(player_id);

				if (inventory) {
					inventory->InsertItem(*collectible);
				}

				break;
			}
		}
	}

	/******************************************************************************/
	/*!
	  \fn UpdateChildOffset()

	  \brief Update child entity's position
	*/
	/******************************************************************************/
	void UpdateChildOffset(const EntityID& parent_id) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		ParentChild* pc = component_mgr->GetComponent<ParentChild>(parent_id);
		Transform* parent_xform = component_mgr->GetComponent<Transform>(parent_id);

		if (!pc || !parent_xform) return;

		for (auto& child : pc->GetChildren()) {

			Transform* child_xform = component_mgr->GetComponent<Transform>(child->GetID());

			if (child_xform) {

				Vector2D updated_pos = child_xform->GetOffset() + parent_xform->GetPosition();
				child_xform->SetPosition(updated_pos);
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn TextureUpdateScript()

	  \brief Texture update script for Player
	*/
	/******************************************************************************/
	void TextureUpdateScript(const EntityID& parent_id) {
		
		std::shared_ptr<GraphicsSystem> graphics = CORE->GetSystem<GraphicsSystem>();
		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();

		AnimationRenderer* renderer = component_mgr->GetComponent<AnimationRenderer>(parent_id);
		Status* status = component_mgr->GetComponent<Status>(parent_id);
		Motion* motion = component_mgr->GetComponent<Motion>(parent_id);
		Name* name = component_mgr->GetComponent<Name>(parent_id);

		// If any pointers are invalid, return
		if (!renderer || !status || !motion || !name)
			return;

		switch (status->GetStatus())
		{
			case StatusType::BURROW:
			{
				renderer->SetAnimationStatus(true);

				if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y)) {

					graphics->ChangeAnimation(renderer, "Player_Burrow_Idle");
				}
				else {

					graphics->ChangeAnimation(renderer, "Player_Burrow_Walk");
				}
				break;
			}
			case StatusType::INVISIBLE:
			{
				if (renderer->FinishedAnimating())
					renderer->SetAnimationStatus(false);

				break;
			}
			default:
			{
				// check if hit then reset accordingly based on the timer

				renderer->SetAnimationStatus(true);

				if (renderer->FinishedAnimating()) {

					// If velocity is essentially 0, set player to idle
					if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y)) {

						graphics->ChangeAnimation(renderer, "Player_Idle");
					}
					// If velocity isn't 0 set to walk
					else {

						graphics->ChangeAnimation(renderer, "Player_Walk");

						if (renderer->FinishedAnimating())
							CORE->GetSystem<SoundSystem>()->PlaySounds("PlayerJump");
					}
				}
			}

		}

		if (motion->GetVelocity().x > 0 && motion->IsLeft()) {

			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {

			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
	}


	/******************************************************************************/
	/*!
	  \fn PlayerControllerScript()

	  \brief Player controller script
	*/
	/******************************************************************************/
	void PlayerControllerScript(const EntityID& id, Message* message) {

		Message_Input* m = dynamic_cast<Message_Input*>(message);

		std::shared_ptr<EntityManager> entity_mgr = CORE->GetManager<EntityManager>();
		std::shared_ptr<ComponentManager> component_mgr = CORE->GetManager<ComponentManager>();
		std::shared_ptr<SoundSystem> sound_system = CORE->GetSystem<SoundSystem>();
		std::shared_ptr<Game> game = CORE->GetSystem<Game>();

		InputController* controller = component_mgr->GetComponent<InputController>(id);
		
		if (!m || !component_mgr || !entity_mgr || !game || !controller)
			return;

		

		if (controller->VerifyKey("pause", m->input_)) { // "Esc" key
			CORE->ToggleCorePauseStatus(); // Disable physics update
			CORE->ToggleGamePauseStatus(); // Toggle game's pause menu
			CORE->GetSystem<Collision>()->ToggleClickables(1);
			CORE->GetSystem<Collision>()->ToggleClickables(3);

			if (m_PlayState.GetHelp()) {

				m_PlayState.SetHelp(false);

				CORE->GetSystem<Collision>()->ToggleClickables(1);
				CORE->GetSystem<Collision>()->ToggleClickables(2);
			}

			if (CORE->GetCorePauseStatus()) {

				Message pause{ MessageIDTypes::BGM_PAUSE };
				CORE->BroadcastMessage(&pause);
			}
			else {

				Message pause{ MessageIDTypes::BGM_RESUME };
				CORE->BroadcastMessage(&pause);
			}
		}
		if (controller->VerifyKey("advance_text", m->input_)) {

			CORE->GetSystem<DialogueSystem>()->AdvanceText();
		}


		if (!entity_mgr->GetPlayerEntities().empty() && !CORE->GetCorePauseStatus()) {

			EntityID player_id = entity_mgr->GetPlayerEntities().back()->GetID();
			Status* player_status = component_mgr->GetComponent<Status>(player_id);
			Transform* player_transform = component_mgr->GetComponent<Transform>(player_id);
			AnimationRenderer* anim_renderer = CORE->GetManager<ComponentManager>()->GetComponent<AnimationRenderer>(player_id);
			
			//God Mode
			if (controller->VerifyKey("god", m->input_)) {

				CORE->ToggleGodMode();
			}

			if (CORE->GetGodMode()) {

				if (controller->VerifyKey("lose", m->input_)) {

					Message msg{ MessageIDTypes::GSM_LOSE };
					CORE->BroadcastMessage(&msg);
				}

				if (controller->VerifyKey("win", m->input_)) {

					Message msg{ MessageIDTypes::GSM_WIN };
					CORE->BroadcastMessage(&msg);
				}

				if (controller->VerifyKey("teleport_key", m->input_)) {

					player_transform->SetPosition( { 24.8f, 31.0f } );
				}

				if (controller->VerifyKey("teleport_house", m->input_)) {

					player_transform->SetPosition( { -23.5f, 23.0f } );
				}
			}

			// Skills
			if (controller->VerifyKey("burrow", m->input_)) {

				if (CORE->GetSystem<Collision>()->UnBurrowReady() && VerifyStatusNoneOrAlt(player_status->GetStatus(), StatusType::BURROW)) {

					m_PlayState.SetStatus("Player", StatusType::BURROW, 0.0f, &*CORE->GetSystem<Game>()); // "N"

					if (player_status->GetStatus() == StatusType::BURROW)
						sound_system->PlaySounds("PlayerBurrowIn");
					else
						sound_system->PlaySounds("PlayerBurrowOut");
				}
			}
			else if (controller->VerifyKey("invisible", m->input_)) {

				if (VerifyStatusNoneOrAlt(player_status->GetStatus(), StatusType::INVISIBLE)) {

					bool hide_ready = CORE->GetSystem<Collision>()->HideReady();

					//to pass, either
					//1. collided with bush
					//2. invisible (already invisible, can just resurface)
					if (hide_ready || player_status->GetStatus() == StatusType::INVISIBLE) {

						if (player_status->GetStatus() == StatusType::INVISIBLE) {

							CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(anim_renderer, "Player_Idle");
						}

						else {

							CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(anim_renderer, "Player_Hiding");
						}

						m_PlayState.SetStatus("Player", StatusType::INVISIBLE, 0.0f, &*CORE->GetSystem<Game>()); // "M"
					}
				}
			}
			

			if (player_status && player_status->GetStatus() != StatusType::INVISIBLE && !CORE->GetMovementLock()) {
				//input group
				float power = component_mgr->GetComponent<Motion>(id)->GetForce();

				if (controller->VerifyKey("move_left", m->input_)) {

					CORE->GetManager<ForcesManager>()->AddForce(player_id, "left", PE_FrameRate.GetFixedDelta(), { -power, 0.0f });
					PlayBurrow(player_status);
				}
				else if (controller->VerifyKey("move_right", m->input_)) {

					CORE->GetManager<ForcesManager>()->AddForce(player_id, "right", PE_FrameRate.GetFixedDelta(), { power, 0.0f });
					PlayBurrow(player_status);
				}
				else if (controller->VerifyKey("move_up", m->input_)) {

					CORE->GetManager<ForcesManager>()->AddForce(player_id, "up", PE_FrameRate.GetFixedDelta(), { 0.0f, power });
					PlayBurrow(player_status);
				}
				else if (controller->VerifyKey("move_down", m->input_)) {

					CORE->GetManager<ForcesManager>()->AddForce(player_id, "down", PE_FrameRate.GetFixedDelta(), { 0.0f, -power });
					PlayBurrow(player_status);
				}
				else if (controller->VerifyKey("spin_left", m->input_)) {

					Transform* player_xform = component_mgr->GetComponent<Transform>(id);
					if (player_xform) {
						RotateLeft(player_xform, true);
					}
				}
				else if (controller->VerifyKey("spin_right", m->input_)) {

					Transform* player_xform = component_mgr->GetComponent<Transform>(id);
					if (player_xform) {
						RotateLeft(player_xform, false);
					}
				}
				else if (controller->VerifyKey("shrink", m->input_)) {

					Scale* player_scale = component_mgr->GetComponent<Scale>(id);
					if (player_scale) {
						ScaleEntityBig(player_scale, false);
					}
				}
				else if (controller->VerifyKey("expand", m->input_)) {

					Scale* player_scale = component_mgr->GetComponent<Scale>(id);
					if (player_scale) {
						ScaleEntityBig(player_scale, true);
					}
				}
			}
		}
	}
}


#endif // !_PLAYER_SCRIPT_H_