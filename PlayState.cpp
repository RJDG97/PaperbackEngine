
#include <iostream>

#include "Game.h"
#include "PlayState.h"
#include "PauseState.h"
#include "MenuState.h"

#include "InputSystem.h"
#include "WinKeyCodes.h"

#include "GraphicsSystem.h"

#include "Core.h"
#include "Factory.h"

// SAMPLE PLAY STATE

PlayState m_PlayState;

void PlayState::init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "PlayState init Successful" << std::endl;
	std::cout << "press SPACE to PAUSE" << std::endl;
	std::cout << "press ESCAPE to return to MAIN MENU" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	//creating duplicate entities
	FACTORY->Create("Entity1");
	FACTORY->Create("Entity2");
}

void PlayState::free()
{
	std::cout << "PlayState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void PlayState::pause()
{
	std::cout << "PlayState Paused" << std::endl;
}

void PlayState::resume()
{
	std::cout << "PlayState Resumed" << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;
}

void PlayState::update(Game* game)
{
	UNREFERENCED_PARAMETER(game);

	/*if (sys_input_.CheckTriggeredInput(PE_N))
	{
		std::cout << "YOU ARE IN THE PLAY STATE" << std::endl;
	}

	if (sys_input_.CheckTriggeredInput(PE_SPACE))
	{
		std::cout << "PAUSING....." << std::endl;
		game->PushState(PauseState::Instance());
	}

	if (sys_input_.CheckTriggeredInput(PE_ESCAPE))
	{
		std::cout << "GOING BACK TO MAIN MENU" << std::endl;
		//game->ChangeState(MenuState::Instance());
	}*/
}

void PlayState::draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}

void PlayState::StateInputHandler(int key_val) {

	switch (key_val)
	{
	case 0x25: //LEFT ARROW key
	{
		std::cout << "Play State: Moving Left" << std::endl;
		Vector2D accel{ -1, 0 };
		MessagePhysics_Accel msg{ MessageIDTypes::PHY_UpdateAccel, accel };
		CORE->BroadcastMessage(&msg);
		//CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->TempMoveCamera();
		break;
	}
	case 0x26: //UP ARROW key
	{
		std::cout << "Play State: Moving Up" << std::endl;
		Vector2D accel{ 0, 1 };
		MessagePhysics_Accel msg{ MessageIDTypes::PHY_UpdateAccel, accel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	case 0x27: //RIGHT ARROW key
	{
		std::cout << "Play State: Moving Right" << std::endl;
		Vector2D accel{ 1, 0 };
		MessagePhysics_Accel msg{ MessageIDTypes::PHY_UpdateAccel, accel };
		CORE->BroadcastMessage(&msg);
		break;
	}
	case 0x28: //DOWN ARROW key
	{
		std::cout << "Play State: Moving Down" << std::endl;
		Vector2D accel{ 0, -1 };
		MessagePhysics_Accel msg{ MessageIDTypes::PHY_UpdateAccel, accel };
		CORE->BroadcastMessage(&msg);
		break;
	}

	}
}
