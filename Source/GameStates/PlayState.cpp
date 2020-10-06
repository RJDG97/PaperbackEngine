
#include <iostream>

#include "Systems/Game.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"

#include "Systems/InputSystem.h"

#include "Systems/GraphicsSystem.h"

#include "Engine/Core.h"
#include "Systems/Factory.h"

// SAMPLE PLAY STATE

PlayState m_PlayState;

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
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Player", EntityTypes::PLAYER);
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
	//to use in play state, in menu state for testing
	//meant to handle game logic components like status
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
				case 69: //E
				{
					SetStatus(EntityTypes::PLAYER, StatusType::INVISIBLE, game);
					break;
				}
				case 82: //R
				{
					SetStatus(EntityTypes::PLAYER, StatusType::BURROW, game);
					break;
				}
			}
		}
	}

}
