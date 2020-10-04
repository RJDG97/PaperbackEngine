#include <iostream>

#include "Systems/Game.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"

#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"

#include "Engine/Core.h" //FOR TESTING

#include "Systems/GraphicsSystem.h"
#include "Systems/Factory.h"

#include "Components/Status.h"

//SAMPLE MENU STATE

//MenuState MenuState::m_MenuState;
MenuState m_MenuState;

void MenuState::Init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	CORE->GetManager<TextureManager>()->TempFunctionForTesting();
	CORE->GetManager<AnimationManager>()->TempFunctionForTesting();

	// Creating base archetype (Temporary stored within main entity array for testing and update purposes)
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Player", EntityTypes::Player);
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Enemy", EntityTypes::Enemy);
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Wall", EntityTypes::Wall);

	//FACTORY->Create("Entity2");
}

void MenuState::Free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void MenuState::Update(Game* game, float frametime)
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

void MenuState::Draw(Game* game)
{
	//GraphicsSystem::Instance().Draw();
	UNREFERENCED_PARAMETER(game);
}

void MenuState::StateInputHandler(unsigned char key_val) {
	(void)key_val;
	//0x25 //LEFT ARROW key
	//0x26 //UP ARROW key
	//0x27 //RIGHT ARROW key
	//0x28 //DOWN ARROW key
	/*
	switch (key_val)
	{
	case 0x25: //LEFT ARROW key
	{
		std::cout << "Play State: Moving Left" << std::endl;
		Vector2D vel{ -10, 0 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		//CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->TempMoveCamera();
		break;
	}
	case 0x26: //UP ARROW key
	{
		std::cout << "Play State: Moving Up" << std::endl;
		Vector2D vel{ 0, 10 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	case 0x27: //RIGHT ARROW key
	{
		std::cout << "Play State: Moving Right" << std::endl;
		Vector2D vel{ 10, 0 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	case 0x28: //DOWN ARROW key
	{
		std::cout << "Play State: Moving Down" << std::endl;
		Vector2D vel{ 0, -10 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	default:

		Vector2D vel{ 0, 0 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}*/


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

	MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, new_vel };
	CORE->BroadcastMessage(&msg);
}