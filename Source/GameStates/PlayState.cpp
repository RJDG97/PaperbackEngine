#include <iostream>

#include "Systems/Game.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"

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

	CORE->ResetCorePauseStatus();
	//CORE->GetManager<TextureManager>()->TextureBatchLoad("Play");
	//CORE->GetManager<AnimationManager>()->AnimationBatchLoad("Play");
	//CORE->GetManager<FontManager>()->FontBatchLoad("Play");
	FACTORY->LoadLevel("Play");
	FACTORY->LoadLevel("Pause");

	CORE->GetManager<AMap>()->InitAMap( CORE->GetManager<EntityManager>()->GetEntities() );
	CORE->GetSystem<PartitioningSystem>()->InitPartition();
}

void PlayState::Free()
{
	std::cout << "PlayState clean Successful" << std::endl;
	
	CORE->GetSystem<ImguiSystem>()->ResetSelectedEntity();
	FACTORY->DestroyAllEntities();
}

void PlayState::Update(Game* game, float frametime)
{
	EntityManager* entity_mgr_ = &*CORE->GetManager<EntityManager>();

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
				status->second->status_ = StatusType::NONE;
			}
		}
	}

	// To use in play state meant to handle game logic components like BasicAI
	for (Game::BasicAIIt basic_ai = game->basicai_arr_->begin(); basic_ai != game->basicai_arr_->end(); ++basic_ai) {

		if (basic_ai->second->num_destinations_ < 1) 
			continue;

		std::shared_ptr<Transform> xform = 
			std::dynamic_pointer_cast<Transform>(basic_ai->second->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));
		DEBUG_ASSERT((xform), "AI does not have Transform component");

		// Check if entity close to destination aka point box collision

		// Calculate distance between ai and destination
		float distance = Vector2DLength(*(basic_ai->second->current_destination_) - xform->position_);
		if (distance <= 1.0f) {

			// if ai is near then calculate new vector and set
			// check if next destination is out of range, and loop to beginning if so
			BasicAI::DestinationIt next_it = basic_ai->second->current_destination_;

			if (++next_it == std::end(basic_ai->second->destinations_)) {

				//if next destination does not exist, then wrap back to beginning
				next_it = basic_ai->second->destinations_.begin();
			}

			basic_ai->second->current_destination_ = next_it;
		}
			
		//get directional unit vector
		Vector2D directional = *basic_ai->second->current_destination_ - xform->position_;
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= basic_ai->second->speed;

		CORE->GetManager<ForcesManager>()->AddForce(basic_ai->second->GetOwner()->GetID(), "movement", frametime, directional);

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
				it->second->status_timer_ == 0.0f) {
				
				it->second->status_ = StatusType::NONE;
			}
		}
	}
}

std::string PlayState::GetStateName() {

	return "Play";
}

void PlayState::StateInputHandler(Message* msg, Game* game) {

	EntityManager* entity_mgr_ = &*CORE->GetManager<EntityManager>();
	ComponentManager* component_mgr_ = &*CORE->GetManager<ComponentManager>();

	if (game) {
		for (Game::InputControllerIt it = game->input_controller_arr_->begin(); it != game->input_controller_arr_->end(); ++it) {
			Message_Input* m = dynamic_cast<Message_Input*>(msg);
			
			if (!m)
				continue;

			InputController* InputController = it->second;
			float power = 40.0f;

			if (game->GetStateName() == "Play") {
			
				if (InputController->VerifyKey("pause", m->input_)) {
					CORE->ToggleCorePauseStatus(); // "ESC"
					CORE->GetSystem<Collision>()->ToggleClickables();
				}

				// Re-enable this if you want to be able to exit the game by pressing enter once pause menu is brought up
				// Yet to include buttons into the play state because we need a way to filter UI for pause menu in graphics
				if (CORE->GetCorePauseStatus() && InputController->VerifyKey("confirm", m->input_)) {
					//CORE->SetGameActiveStatus(false);
					game->ChangeState(&m_MenuState);
					return;
				}
			}

			if (!entity_mgr_->GetPlayerEntities().empty() && !CORE->GetCorePauseStatus()) {

				EntityID player_id = entity_mgr_->GetPlayerEntities().back()->GetID();
				Status* player_status = component_mgr_->GetComponent<Status>(player_id);
				AnimationRenderer* anim = component_mgr_->GetComponent<AnimationRenderer>(player_id);

				// Skills
				if (InputController->VerifyKey("burrow", m->input_)) {

					if (CORE->GetSystem<Collision>()->BurrowReady()) {
						SetStatus("Player", StatusType::BURROW, 0.0f, &*CORE->GetSystem<Game>()); // "M"
						CORE->GetSystem<GraphicsSystem>()->FlipTextureX(anim);
					}
				}
				else if (InputController->VerifyKey("invisible", m->input_)) {
					SetStatus("Player", StatusType::INVISIBLE, 0.0f, &*CORE->GetSystem<Game>()); // "N"
				}

				if (player_status && player_status->status_ != StatusType::INVISIBLE) {
					//input group
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
				}
			}
		}

		if (!game->debug_ && game->GetStateName() == "Play") {

			switch (msg->message_id_) {
				//check for collision between button & mouse
			case MessageIDTypes::BUTTON: {

				Message_Button* m = dynamic_cast<Message_Button*>(msg);

				if (m->button_index_ == 1) {

					//true returned, trigger scene change
					game->ChangeState(&m_MenuState);
					return;
				}

				if (m->button_index_ == 2) {

					game->ChangeState(&m_MenuState);
					//CORE->GetSystem<ImguiSystem>()->SetImguiBool(true);
					return;
				}

				if (m->button_index_ == 3) {

					game->ChangeState(&m_MenuState);
					//CORE->SetGameActiveStatus(false);
					return;
				}
			}
			}
		}
	}
	else {

		if (msg->message_id_ == MessageIDTypes::C_MOVEMENT) {

			Message_PlayerInput* m = dynamic_cast<Message_PlayerInput*>(msg);
			assert(m != nullptr && "Message is not a player input message");
			unsigned char key_val = static_cast<unsigned char>(m->input_flag_);

			// set up velocity based input flag value
			Vec2 new_vel{};
			float power = 5.0f;

			if (key_val & W_FLAG) {

				new_vel.y += power;
			}

			if (key_val & S_FLAG) {

				new_vel.y -= power;
			}

			if (key_val & A_FLAG) {

				new_vel.x -= power;
			}

			if (key_val & D_FLAG) {

				new_vel.x += power;
			}

			//std::cout << "New Velocity Passed: " << new_vel.x << ", " << new_vel.y << std::endl;

			MessagePhysics_Motion m2{ MessageIDTypes::CAM_UPDATE_POS, new_vel };
			CORE->BroadcastMessage(&m2);
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