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

#include "Components/Transform.h"

//SAMPLE MENU STATE

//MenuState MenuState::m_MenuState;
MenuState m_MenuState;
Entity* start_blok{};
Entity* crash_blok{};

void MenuState::Init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	// Creating base archetype (Temporary stored within main entity array for testing and update purposes)
	//FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Player", EntityTypes::Player);
	//FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Enemy", EntityTypes::Enemy);
	
	//start_blok = FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Wall", EntityTypes::WALL);
	//crash_blok = FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "ButtonCrash", EntityTypes::WALL);

	start_blok = FACTORY->CloneArchetype("Wall");
	crash_blok = FACTORY->CloneArchetype("ButtonCrash");
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

void MenuState::StateInputHandler(Message* msg, Game* game) {

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

	if (!game && msg->message_id_ == MessageIDTypes::M_MOVEMENT) {

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

	if (game && msg->message_id_ == MessageIDTypes::M_MOUSE_PRESS) {

		//check for collision between button & mouse
		Vector2D cursor = CORE->GetSystem<InputSystem>()->GetCursorPosition();
		M_DEBUG->WriteDebugMessage("Cursor Location: " + std::to_string(cursor.x) + ", " + std::to_string(cursor.y) + "\n");

		if (CORE->GetSystem<Collision>()->CheckCursorCollision(CORE->GetSystem<InputSystem>()->GetCursorPosition(), start_blok->GetID())) {

			//true returned, trigger scene change
			game->ChangeState(&m_PlayState);
			return;
		}

		if (CORE->GetSystem<Collision>()->CheckCursorCollision(CORE->GetSystem<InputSystem>()->GetCursorPosition(), crash_blok->GetID())) {

			DEBUG_ASSERT(false, "Forced Crash Triggered");
		}
	}
}