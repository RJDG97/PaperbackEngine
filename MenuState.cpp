#include <iostream>

#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"

#include "InputSystem.h"
#include "WinKeyCodes.h"
#include "WindowsSystem.h"

//SAMPLE MENU STATE

MenuState MenuState::m_MenuState;

void MenuState::init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;
}

void MenuState::free()
{
	std::cout << "MenuState clean Successful" << std::endl;
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

	if (sys_input.CheckTriggeredInput(PE_M))
	{
		std::cout << "you are in the MAIN MENU" << std::endl;
	}	
	
	if (sys_input.CheckTriggeredInput(PE_SPACE))
	{
		std::cout << "GAME STARTING....." << std::endl;
		game->ChangeState(PlayState::Instance());
	}
	if (sys_input.CheckTriggeredInput(PE_ESCAPE))
	{
		pause();
	}

	if (sys_input.CheckTriggeredInput(PE_Y))
	{
		game->Quit();
	}	
	
	if (sys_input.CheckTriggeredInput(PE_N))
	{
		resume();
	}

}

void MenuState::draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}


