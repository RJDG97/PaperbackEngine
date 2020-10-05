
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
				std::cout << "Reducing status timer" << std::endl;
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

void PlayState::StateInputHandler(unsigned char key_val, Game* game) {
	UNREFERENCED_PARAMETER(game);

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

	MessagePhysics_Motion msg{ MessageIDTypes::PHY_UPDATE_VEL, new_vel };
	CORE->BroadcastMessage(&msg);
}
