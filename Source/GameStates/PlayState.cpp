
#include <iostream>

#include "Systems/Game.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"

#include "Systems/InputSystem.h"

#include "Systems/GraphicsSystem.h"

#include "Engine/Core.h"
#include "Systems/Factory.h"


#include "Components/Transform.h"
#include "Components/Motion.h"

#include "Entity/ComponentTypes.h"

#include <memory>

// SAMPLE PLAY STATE
PlayState m_PlayState;


// Temporary pre-declaration for Engine Proof
// Yeah its pretty illegal I know
void ScaleEntityBig(std::shared_ptr<Scale> scale, bool yes);
void RotateLeft(std::shared_ptr<Transform> xform, bool yes);

//demo pointer to player
Entity* player;

void PlayState::Init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "PlayState init Successful" << std::endl;
	std::cout << "press SPACE to PAUSE" << std::endl;
	std::cout << "press ESCAPE to return to MAIN MENU" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	CORE->GetManager<TextureManager>()->TempFunctionForTesting();
	CORE->GetManager<AnimationManager>()->TempFunctionForTesting();

	// Creating base archetype (Temporary stored within main entity array for testing and update purposes)
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Floor", EntityTypes::FLOOR);
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "MovingWall", EntityTypes::WALL);
	player = FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Player", EntityTypes::PLAYER);
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Enemy", EntityTypes::ENEMY);
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Wall", EntityTypes::WALL);
}

void PlayState::Free()
{
	std::cout << "PlayState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void PlayState::Update(Game* game, float frametime)
{
	// To use in play state, in menu state for testing
	// meant to handle game logic components like Status
	for (Game::StatusIt status = game->status_arr_.begin(); status != game->status_arr_.end(); ++status) {

		if (status->second->status_ != StatusType::NONE) {

			if (status->second->status_timer_ > 0.0f) {
				status->second->status_timer_ -= frametime;
				//std::cout << "Reducing status timer" << std::endl;
			}
			else {
				std::cout << "Resetting status type to none" << std::endl;
				status->second->status_ = StatusType::NONE;
			}
		}
	}

	// To use in play state meant to handle game logic components like BasicAI
	for (Game::BasicAIIt basic_ai = game->basicai_arr_.begin(); basic_ai != game->basicai_arr_.end(); ++basic_ai) {

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

			if (++next_it == basic_ai->second->destinations_.end()) {

				//if next destination does not exist, then wrap back to beginning
				next_it = basic_ai->second->destinations_.begin();
			}

			//get directional unit vector
			Vector2D directional = *next_it - *basic_ai->second->current_destination_;
			directional /= Vector2DLength(directional);

			//multiply by speed
			directional *= basic_ai->second->speed;

			//set vector
			std::shared_ptr<Motion> motion = 
				std::dynamic_pointer_cast<Motion>(basic_ai->second->GetOwner()->GetComponent(ComponentTypes::MOTION));
			DEBUG_ASSERT((motion), "AI does not have a Motion component");

			motion->velocity_ = directional;

			basic_ai->second->current_destination_ = next_it;
		}
		else {

			//set directional vector
		}
	}
}

void PlayState::Draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}

void PlayState::SetStatus(EntityTypes entity_type, StatusType status_type, Game* game) {
	
	for (Game::StatusIt it = game->status_arr_.begin(); it != game->status_arr_.end(); ++it) {

		if (it->second->GetOwner()->GetType() == entity_type && it->second->status_ == StatusType::NONE) {
			
			it->second->status_ = status_type;
			it->second->status_timer_ = 3.0f; // change timer accordingly in the future
		}
	}
}

void PlayState::StateInputHandler(Message* msg, Game* game) {
	//UNREFERENCED_PARAMETER(game);

	if (!game) {

		if (msg->message_id_ == MessageIDTypes::M_MOVEMENT) {
			
			Message_PlayerInput* m = dynamic_cast<Message_PlayerInput*>(msg);
			assert(m != nullptr && "Message is not a player input message");
			unsigned char key_val = m->input_flag_;

			// set up velocity based input flag value
			Vec2 new_vel{};

			if (key_val & UP_FLAG) {

				new_vel.y += 100.0f;
			}

			if (key_val & DOWN_FLAG) {

				new_vel.y -= 100.0f;
			}

			if (key_val & LEFT_FLAG) {

				new_vel.x -= 100.0f;
			}

			if (key_val & RIGHT_FLAG) {

				new_vel.x += 100.0f;
			}

			//std::cout << "New Velocity Passed: " << new_vel.x << ", " << new_vel.y << std::endl;

			MessagePhysics_Motion m2{ MessageIDTypes::PHY_UPDATE_VEL, new_vel };
			CORE->BroadcastMessage(&m2);
		}


	}

	if (game) {

		if (msg->message_id_ == MessageIDTypes::M_BUTTON_PRESS) {
			
			Message_PlayerInput* m = dynamic_cast<Message_PlayerInput*>(msg);
			assert(m != nullptr && "Message is not a player input message");
			unsigned char key_val = m->input_flag_;

			switch (key_val)
			{
				case GLFW_KEY_E: // "E"
				{
					SetStatus(EntityTypes::PLAYER, StatusType::INVISIBLE, game);
					break;
				}
				case GLFW_KEY_R: // "R"
				{
					SetStatus(EntityTypes::PLAYER, StatusType::BURROW, game);
					break;
				}
				case GLFW_KEY_COMMA: // "<"
				{

					// PLACEHOLDER REMOVE THIS AFTER ENGINE PROOF

					std::shared_ptr<Scale> player_scale = std::dynamic_pointer_cast<Scale>(player->GetComponent(ComponentTypes::SCALE));
					ScaleEntityBig(player_scale, false);
					break;
				}
				case GLFW_KEY_PERIOD: // ">"
				{
					std::shared_ptr<Scale> player_scale = std::dynamic_pointer_cast<Scale>(player->GetComponent(ComponentTypes::SCALE));
					ScaleEntityBig(player_scale, true);
					break;
				}
				case GLFW_KEY_SEMICOLON: // ";"
				{
					std::shared_ptr<Transform> player_xform = std::dynamic_pointer_cast<Transform>(player->GetComponent(ComponentTypes::TRANSFORM));
					RotateLeft(player_xform, true);
					break;
				}
				case GLFW_KEY_APOSTROPHE: // "'"
				{
					std::shared_ptr<Transform> player_xform = std::dynamic_pointer_cast<Transform>(player->GetComponent(ComponentTypes::TRANSFORM));
					RotateLeft(player_xform, false);
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

}

void ScaleEntityBig(std::shared_ptr<Scale> scale, bool yes) {
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

void RotateLeft(std::shared_ptr<Transform> xform, bool yes) {
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