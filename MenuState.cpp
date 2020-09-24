#include <iostream>

#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"

#include "InputSystem.h"
#include "WinKeyCodes.h"
#include "WindowsSystem.h"

#include "GraphicsSystem.h"
#include "Factory.h"

//SAMPLE MENU STATE

//MenuState MenuState::m_MenuState;
MenuState m_MenuState;

void MenuState::init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	//creating duplicate entities
	FACTORY->Create("Entity1");
	FACTORY->Create("Entity2");
}

void MenuState::free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void MenuState::pause()
{
	std::cout << "CONFIRM???" << std::endl << "Y/N" << std::endl;
}

void MenuState::resume()
{
	std::cout << "MenuState Resumed" << std::endl;
}

void MenuState::update(Game* game)
{
	// input test
	UNREFERENCED_PARAMETER(game);

	/*if (sys_input_.CheckTriggeredInput(PE_M))
	{
		std::cout << "you are in the MAIN MENU" << std::endl;
	}	
	
	if (sys_input_.CheckTriggeredInput(PE_SPACE))
	{
		std::cout << "GAME STARTING....." << std::endl;
		//game->ChangeState(PlayState::Instance());
	}
	if (sys_input_.CheckTriggeredInput(PE_ESCAPE))
	{
		pause();
	}

	if (sys_input_.CheckTriggeredInput(PE_Y))
	{
		game->Quit();
	}	
	
	if (sys_input_.CheckTriggeredInput(PE_N))
	{
		resume();
	}*/

}

void MenuState::draw(Game* game)
{
	GraphicsSystem::Instance().Draw();
	UNREFERENCED_PARAMETER(game);
}

void MenuState::StateInputHandler(int key_val) {
	(void)key_val;
	//0x25 //LEFT ARROW key
	//0x26 //UP ARROW key
	//0x27 //RIGHT ARROW key
	//0x28 //DOWN ARROW key

}