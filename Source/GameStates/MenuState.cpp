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
#include "Systems/Collision.h"

#include "Components/Status.h"

//SAMPLE MENU STATE

//MenuState MenuState::m_MenuState;
MenuState m_MenuState;
Entity* start_blok{};

void MenuState::Init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	//CORE->GetManager<TextureManager>()->TempFunctionForTesting();
	//CORE->GetManager<AnimationManager>()->TempFunctionForTesting();

	// Creating base archetype (Temporary stored within main entity array for testing and update purposes)
	//FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Player", EntityTypes::Player);
	//FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Enemy", EntityTypes::Enemy);
	start_blok = FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Wall", EntityTypes::WALL);
}

void MenuState::Free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void MenuState::Update(Game* game, float frametime)
{
	UNREFERENCED_PARAMETER(game);
	UNREFERENCED_PARAMETER(frametime);
}

void MenuState::Draw(Game* game)
{
	//GraphicsSystem::Instance().Draw();
	UNREFERENCED_PARAMETER(game);
}

void MenuState::StateInputHandler(unsigned char key_val, Game* game) {
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

	if (game) {
		switch (key_val)
		{
			case 16:
			{
				//check for collision between button & mouse

				if (CORE->GetSystem<Collision>()->CheckCursorCollision(CORE->GetSystem<InputSystem>()->GetCursorPosition(), start_blok->GetID())) {

					//true returned, trigger scene change
					game->ChangeState(&m_PlayState);
				}
				break;
			}
		}
	}

	//std::cout << "New Velocity Passed: " << new_vel.x << ", " << new_vel.y << std::endl;

	MessagePhysics_Motion msg{ MessageIDTypes::PHY_UPDATE_VEL, new_vel };
	CORE->BroadcastMessage(&msg);
}