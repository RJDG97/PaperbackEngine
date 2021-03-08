/**********************************************************************************
*\file         PlayState.cpp
*\brief        Contains definition of Play State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include <iostream>

#include "Systems/Game.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"
#include "GameStates/WinLoseState.h"

#include "Systems/InputSystem.h"

#include "Engine/Core.h"
#include "Systems/Factory.h"
#include "Systems/ImguiSystem.h"
#include "Systems/Partitioning.h"
#include "Systems/Collision.h"

#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/Name.h"
#include "Components/AnimationRenderer.h"
#include "Components/Motion.h"

#include "Entity/ComponentTypes.h"

#include "Manager/ForcesManager.h"
#include "Manager/TransitionManager.h"
#include "Manager/AMap.h"

#include "Systems/DialogueSystem.h"
#include "Systems/Parenting.h"

#include <memory>

// Play state
PlayState m_PlayState;

// Temp declaration
void ScaleEntityBig(Scale* scale, bool yes);
void RotateLeft(Transform* xform, bool yes);



void PlayState::Init(std::string)
{
	help_ = false;
	lose_ = false;
	timer_ = 5.0f;

	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();
	logic_mgr_ = &*CORE->GetManager<LogicManager>();
	logic_arr_ = component_mgr_->GetComponentArray<LogicComponent>();
	input_arr_ = component_mgr_->GetComponentArray<InputController>();

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	CORE->GetManager<LayerManager>()->LoadLevelLayers("Play");
	FACTORY->LoadLevel("Play");
	FACTORY->LoadLevel("Pause");

	CORE->GetSystem<GraphicsSystem>()->EnableLighting(true);
	
	MessageBGM_Play msg{ "River" };
	CORE->BroadcastMessage(&msg);

	CORE->GetManager<AMap>()->InitAMap( CORE->GetManager<EntityManager>()->GetEntities() );
	CORE->GetSystem<PartitioningSystem>()->InitPartition();
	CORE->GetSystem<ParentingSystem>()->LinkParentAndChild();
	CORE->GetSystem<CameraSystem>()->CameraZoom(CORE->GetSystem<CameraSystem>()->GetMainCamera(), 0.7f);
	CORE->GetSystem<CameraSystem>()->TargetPlayer();

	CORE->GetManager<DialogueManager>()->LoadDialogueSet("Play");
	//CORE->GetSystem<DialogueSystem>()->SetCurrentDialogue("beginning");
}

void PlayState::Free()
{
	std::cout << "PlayState clean Successful" << std::endl;

	CORE->GetSystem<SoundSystem>()->StopSound("All", true);

	CORE->GetSystem<ImguiSystem>()->ResetSelectedEntity();
	FACTORY->DestroyAllEntities();

	CORE->GetSystem<DialogueSystem>()->TempCleanup();
	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	win_ = false;
	help_ = false;
	lose_ = false;
}

void PlayState::Update(Game* game, float frametime)
{
	if (!entity_mgr_)
		entity_mgr_ = &*CORE->GetManager<EntityManager>();

	if (win_) {
		
		game->ChangeState(&m_WinLoseState, "Win");
		return;
	}

	if (lose_) {
		
		game->ChangeState(&m_WinLoseState, "Lose");
		return;
	}
	entity_mgr_->GetEntities();

	// To use in play state, in menu state for testing
	// meant to handle game logic components like Status
	for (Game::StatusIt status = game->status_arr_->begin(); status != game->status_arr_->end(); ++status) {

		if (status->second->status_ != StatusType::NONE &&
			status->second->status_ != StatusType::INVISIBLE &&
			status->second->status_ != StatusType::BURROW) {

			if (status->second->status_timer_ > 0.0f) {
				status->second->status_timer_ -= frametime;
			}
			else {
				std::cout << "Resetting status type to none" << std::endl;
				status->second->status_timer_ = 0.0f;
				status->second->status_ = StatusType::NONE;
			}
		}
	}

	// To use in play state meant to handle game logic components like BasicAI
	for (Game::BasicAIIt basic_ai = game->basicai_arr_->begin(); basic_ai != game->basicai_arr_->end(); ++basic_ai) {

		basic_ai->second->Update(frametime);
	}

	// If there exists at least 1 player
	if (entity_mgr_->GetPlayerEntities()) {

		if (!component_mgr_)
			component_mgr_ = &*CORE->GetManager<ComponentManager>();

		EntityID player_id = entity_mgr_->GetPlayerEntities()->GetID();
		Health* health = component_mgr_->GetComponent<Health>(player_id);
		Status* status = component_mgr_->GetComponent<Status>(player_id);

		// Only decrement timer when player is burrowed
		if (status->GetStatus() == StatusType::BURROW)
			timer_ -= frametime;

		if (!CORE->GetGodMode() && timer_ < 0.0f) {

			int new_hp = health->GetCurrentHealth() - 1;
			health->SetCurrentHealth(new_hp);
			timer_ = 5.0f;
		}

		if (health && health->GetCurrentHealth() <= 0) {

			CORE->GetSystem<SoundSystem>()->PlaySounds("PlayerDies_0");
			game->ChangeState(&m_WinLoseState, "Lose");
		}
	}
}

void PlayState::Draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}

void PlayState::SetStatus(std::string entity_name, StatusType status_type, float status_length, Game* game) {
	
	for (Game::StatusIt it = game->status_arr_->begin(); it != game->status_arr_->end(); ++it) {
		
		std::string name = ENTITYNAME(it->second->GetOwner()); // Check for assert here potentially (JSON formatting)

		if (name == entity_name && it->second->status_ == StatusType::NONE) {
			
			it->second->status_ = status_type;
			it->second->status_timer_ = status_length;
		}
		else {
			// Double check this condition
			if (it->second->status_ == status_type &&
				it->second->status_timer_ <= 0.0f) {
				
				it->second->status_timer_ = 0.0f;
				it->second->status_ = StatusType::NONE;				
			}
		}
	}
}

std::string PlayState::GetStateName() {

	return "Play";
}

bool PlayState::GetHelp() {
	
	return help_;
}

void PlayState::SetHelp(const bool& status) {
	
	help_ = status;
}

void PlayState::Init(){

	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();
	logic_mgr_ = &*CORE->GetManager<LogicManager>();
	logic_arr_ = component_mgr_->GetComponentArray<LogicComponent>();
	input_arr_ = component_mgr_->GetComponentArray<InputController>();
}

bool VerifyStatusNoneOrAlt(StatusType player, StatusType to_check) {

	return (player == StatusType::NONE || player == to_check);
}

void PlayState::StateInputHandler(Message* msg, Game* game) {

	if (!entity_mgr_)
		entity_mgr_ = &*CORE->GetManager<EntityManager>();

	if (!component_mgr_)
		component_mgr_ = &*CORE->GetManager<ComponentManager>();

	if (game) {
		
		// Logic component update for input controller (Currently only for player)
		for (auto& [id, controller] : *input_arr_) {
			
			LogicComponent* logic = component_mgr_->GetComponent<LogicComponent>(id);

			if (logic) {
				// If the logic is valid, run update loop
				std::string update = logic->GetLogic("UpdateInput");
				logic_mgr_->Exec(update, id, msg);
			}
		}
		
		if (!game->debug_ && game->GetStateName() == "Play") {

			switch (msg->message_id_) {
				//check for collision between button & mouse
			case MessageIDTypes::BUTTON: {

				Message_Button* m = dynamic_cast<Message_Button*>(msg);

				switch (m->button_index_)
				{
				case 9:
				case 1:
				{
					if (help_)
						break;

					CORE->ToggleCorePauseStatus(); // Disable physics update
					CORE->ToggleGamePauseStatus(); // Toggle game's pause menu
					CORE->GetSystem<Collision>()->ToggleClickables(1);
					CORE->GetSystem<Collision>()->ToggleClickables(3);

					if (CORE->GetCorePauseStatus()) {
						
						Message pause{ MessageIDTypes::BGM_PAUSE };
						CORE->BroadcastMessage(&pause);
					}
					else {

						Message pause{ MessageIDTypes::BGM_RESUME };
						CORE->BroadcastMessage(&pause);
					}

					break;
				}
				case 7:
				case 2:
				{
					
					MessageBGM_Play button{ "ButtonPress" };
					CORE->BroadcastMessage(&button);

					CORE->GetSystem<Collision>()->ToggleClickables(2);
					CORE->GetSystem<Collision>()->ToggleClickables(1);
					help_ = !help_;
					break;
				}
				case 3:
				{
					if (help_)
						break;

					MessageBGM_Play button{ "ButtonPress" };
					CORE->BroadcastMessage(&button);
					CORE->GetSystem<DialogueSystem>()->TempCleanup();
					CORE->GetManager<TransitionManager>()->ResetTransition("Default", &m_MenuState);
					return;
					break;
				}
				case 4:
				{
					if (help_)
						break;

					Message mesg{ MessageIDTypes::EXIT };
					CORE->BroadcastMessage(&mesg);
					break;
				}
				case 8:
				{

					CORE->GetSystem<WindowsSystem>()->ToggleFullScreen();
					break;
				}
				}
			}
			}
		}
	}
	else {

		switch (msg->message_id_)
		{
		case MessageIDTypes::C_MOVEMENT:
		{

			Message_PlayerInput* m = dynamic_cast<Message_PlayerInput*>(msg);
			assert(m != nullptr && "Message is not a player input message");
			unsigned char key_val = static_cast<unsigned char>(m->input_flag_);

			// set up velocity based input flag value
			Vec2 new_vel{};
			float power = 10.0f;

			if (key_val & UP_FLAG) {

				new_vel.y += power;
			}

			if (key_val & DOWN_FLAG) {

				new_vel.y -= power;
			}

			if (key_val & LEFT_FLAG) {

				new_vel.x -= power;
			}

			if (key_val & RIGHT_FLAG) {

				new_vel.x += power;
			}

			MessagePhysics_Motion m2{ MessageIDTypes::CAM_UPDATE_POS, new_vel };
			CORE->BroadcastMessage(&m2);
			break;
		}
		case MessageIDTypes::GSM_WIN:
		{

			win_ = true;
			break;
		}
		case MessageIDTypes::GSM_LOSE:
		{

			lose_ = true;
			break;
		}
		}
	}
}

void ScaleEntityBig(Scale* scale, bool yes) {

	Vector2D new_scale;

	if (yes) {

		new_scale = { 5.0f, 5.0f };
	}
	else {
		new_scale = { -5.0f, -5.0f };
	}

	new_scale += scale->GetScale();
	scale->SetScale(new_scale);
}

void RotateLeft(Transform* xform, bool yes) {
	float new_rotation;

	if (yes) {
		new_rotation = 1.0f;
	}
	else {
		new_rotation = -1.0f;
	}

	new_rotation += xform->GetRotation();
	xform->SetRotation(new_rotation);
}