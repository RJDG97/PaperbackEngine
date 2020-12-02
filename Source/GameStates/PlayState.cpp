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
#include "Manager/AMap.h"

#include <memory>

// SAMPLE PLAY STATE
PlayState m_PlayState;


// Temporary pre-declaration for Engine Proof
// Yeah its pretty illegal I know
void ScaleEntityBig(Scale* scale, bool yes);
void RotateLeft(Transform* xform, bool yes);

//demo pointer to player
//Entity* player;

void PlayState::Init(std::string)
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "PlayState init Successful" << std::endl;
	std::cout << "press SPACE to PAUSE" << std::endl;
	std::cout << "press ESCAPE to return to MAIN MENU" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	help_ = false;

	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();
	logic_mgr_ = &*CORE->GetManager<LogicManager>();
	logic_arr_ = component_mgr_->GetComponentArray<LogicComponent>();
	input_arr_ = component_mgr_->GetComponentArray<InputController>();

	CORE->ResetCorePauseStatus();
	FACTORY->LoadLevel("Play");
	FACTORY->LoadLevel("Pause");

	// Not sure...
	CORE->GetSystem<GraphicsSystem>()->EnableLighting(true);
	
	MessageBGM_Play msg{ "GameBGM" };
	CORE->BroadcastMessage(&msg);

	CORE->GetManager<AMap>()->InitAMap( CORE->GetManager<EntityManager>()->GetEntities() );
	CORE->GetSystem<PartitioningSystem>()->InitPartition();

	CORE->GetSystem<CameraSystem>()->CameraZoom(CORE->GetSystem<CameraSystem>()->GetMainCamera(), 0.5f);
}

void PlayState::Free()
{
	std::cout << "PlayState clean Successful" << std::endl;

	MessageBGM_Stop msg{ "GameBGM" };
	CORE->BroadcastMessage(&msg);

	CORE->GetSystem<ImguiSystem>()->ResetSelectedEntity();
	FACTORY->DestroyAllEntities();

	CORE->ResetGodMode();

	win_ = false;
	help_ = false;
}

void PlayState::Update(Game* game, float frametime)
{
	if (!entity_mgr_)
		entity_mgr_ = &*CORE->GetManager<EntityManager>();

	if (win_)
		game->ChangeState(&m_WinLoseState, "Win");

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
	if (entity_mgr_->GetPlayerEntities().size() > 0) {

		if (!component_mgr_)
			component_mgr_ = &*CORE->GetManager<ComponentManager>();

		EntityID player_id = entity_mgr_->GetPlayerEntities().back()->GetID();
		Health* health = component_mgr_->GetComponent<Health>(player_id);

		if (health && health->GetCurrentHealth() <= 0) {
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

		/*
		for (Game::InputControllerIt it = game->input_controller_arr_->begin(); it != game->input_controller_arr_->end(); ++it) {
			Message_Input* m = dynamic_cast<Message_Input*>(msg);
			
			if (!m)
				continue;

			InputController* InputController = it->second;

			if (InputController->VerifyKey("pause", m->input_)) { // "Esc" key
				CORE->ToggleCorePauseStatus(); // Disable physics update
				CORE->ToggleGamePauseStatus(); // Toggle game's pause menu
				CORE->GetSystem<Collision>()->ToggleClickables(1);
				CORE->GetSystem<Collision>()->ToggleClickables(3);

				if (help_) {

					help_ = false;

					CORE->GetSystem<Collision>()->ToggleClickables(1);
					CORE->GetSystem<Collision>()->ToggleClickables(2);
				}

				Message pause{ MessageIDTypes::BGM_PAUSE };
				CORE->BroadcastMessage(&pause);
			}

			if (game->GetStateName() == "Play") {

				// Re-enable this if you want to be able to exit the game by pressing enter once pause menu is brought up
				// Yet to include buttons into the play state because we need a way to filter UI for pause menu in graphics
				//if (CORE->GetCorePauseStatus() && InputController->VerifyKey("confirm", m->input_)) {
				//	//CORE->SetGameActiveStatus(false);
				//	game->ChangeState(&m_MenuState);
				//	return;
				//}
			}

			if (!entity_mgr_->GetPlayerEntities().empty() && !CORE->GetCorePauseStatus()) {

				EntityID player_id = entity_mgr_->GetPlayerEntities().back()->GetID();
				Status* player_status = component_mgr_->GetComponent<Status>(player_id);

				// Skills
				if (InputController->VerifyKey("burrow", m->input_)) {

					if (CORE->GetSystem<Collision>()->BurrowReady() && VerifyStatusNoneOrAlt(player_status->GetStatus(), StatusType::BURROW)) {

						SetStatus("Player", StatusType::BURROW, 0.0f, &*CORE->GetSystem<Game>()); // "N"
					}
				}
				else if (InputController->VerifyKey("invisible", m->input_)) {

					AnimationRenderer* anim_renderer = CORE->GetManager<ComponentManager>()->GetComponent<AnimationRenderer>(player_id);

					if (VerifyStatusNoneOrAlt(player_status->GetStatus(), StatusType::INVISIBLE)) {

						if (player_status->GetStatus() == StatusType::INVISIBLE) {

							CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(anim_renderer, "Player_Idle");
						}

						else {

							CORE->GetSystem<GraphicsSystem>()->ChangeAnimation(anim_renderer, "Player_Hiding");
						}

						SetStatus("Player", StatusType::INVISIBLE, 0.0f, &*CORE->GetSystem<Game>()); // "M"
					}
				}

				if (player_status && player_status->status_ != StatusType::INVISIBLE) {
					//input group
					float power = component_mgr_->GetComponent<Motion>(player_id)->force_;

					if (InputController->VerifyKey("move_left", m->input_)) {

						CORE->GetManager<ForcesManager>()->AddForce(player_id, "left", PE_FrameRate.GetFixedDelta(), { -power, 0.0f });
					}
					else if (InputController->VerifyKey("move_right", m->input_)) {

						CORE->GetManager<ForcesManager>()->AddForce(player_id, "right", PE_FrameRate.GetFixedDelta(), { power, 0.0f });
					}
					else if (InputController->VerifyKey("move_up", m->input_)) {

						CORE->GetManager<ForcesManager>()->AddForce(player_id, "up", PE_FrameRate.GetFixedDelta(), { 0.0f, power });
					}
					else if (InputController->VerifyKey("move_down", m->input_)) {

						CORE->GetManager<ForcesManager>()->AddForce(player_id, "down", PE_FrameRate.GetFixedDelta(), { 0.0f, -power });
					}
					else if (InputController->VerifyKey("spin_left", m->input_)) {

						Transform* player_xform = component_mgr_->GetComponent<Transform>(player_id);
						if (player_xform) {
							RotateLeft(player_xform, true);
						}
					}
					else if (InputController->VerifyKey("spin_right", m->input_)) {

						Transform* player_xform = component_mgr_->GetComponent<Transform>(player_id);
						if (player_xform) {
							RotateLeft(player_xform, false);
						}
					}
					else if (InputController->VerifyKey("shrink", m->input_)) {

						Scale* player_scale = component_mgr_->GetComponent<Scale>(player_id);
						if (player_scale) {
							ScaleEntityBig(player_scale, false);
						}
					}
					else if (InputController->VerifyKey("expand", m->input_)) {

						Scale* player_scale = component_mgr_->GetComponent<Scale>(player_id);
						if (player_scale) {
							ScaleEntityBig(player_scale, true);
						}
					}
					else if (InputController->VerifyKey("advance_text", m->input_)) {

						
					}
				}
			}
		}
		*/
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

					Message pause{ MessageIDTypes::BGM_PAUSE };
					CORE->BroadcastMessage(&pause);
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

					game->ChangeState(&m_MenuState);
					//CORE->SetGameActiveStatus(false);
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

			//std::cout << "New Velocity Passed: " << new_vel.x << ", " << new_vel.y << std::endl;

			MessagePhysics_Motion m2{ MessageIDTypes::CAM_UPDATE_POS, new_vel };
			CORE->BroadcastMessage(&m2);
			break;
		}
		case MessageIDTypes::GSM_WIN:
		{

			win_ = true;
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