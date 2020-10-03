
#include <iostream>

#include "Systems/Game.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"

#include "Systems/InputSystem.h"
#include "WinKeyCodes.h"

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

	// Creating base archetype (Temporary stored within main entity array for testing and update purposes)
	FACTORY->CreateAndSerializeArchetype("Resources/EntityConfig/2compTest.json", "Player", EntityTypes::Player);

	//FACTORY->Create("Entity2");
}

void PlayState::Free()
{
	std::cout << "PlayState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void PlayState::Update(Game* game, float frametime)
{
	UNREFERENCED_PARAMETER(game);

}

void PlayState::Draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}

void PlayState::StateInputHandler(unsigned char key_val) {

	/*switch (key_val)
	{
	case 0x25: //LEFT ARROW key
	{
		std::cout << "Play State: Moving Left" << std::endl;
		Vector2D vel{ -1, 0 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		//CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->TempMoveCamera();
		break;
	}
	case 0x26: //UP ARROW key
	{
		std::cout << "Play State: Moving Up" << std::endl;
		Vector2D vel{ 0, 1 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	case 0x27: //RIGHT ARROW key
	{
		std::cout << "Play State: Moving Right" << std::endl;
		Vector2D vel{ 1, 0 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	case 0x28: //DOWN ARROW key
	{
		std::cout << "Play State: Moving Down" << std::endl;
		Vector2D vel{ 0, -1 };
		MessagePhysics_Motion msg{ MessageIDTypes::PHY_UpdateVel, vel };
		CORE->BroadcastMessage(&msg);
		break;
	}

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
