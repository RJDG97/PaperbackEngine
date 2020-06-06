
#include <iostream>

#include "Game.h"
#include "PlayState.h"
#include "PauseState.h"
#include "MenuState.h"

#include "InputSystem.h"
#include "WinKeyCodes.h"

// SAMPLE PLAY STATE

PlayState PlayState::m_PlayState;

void PlayState::init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "PlayState init Successful" << std::endl;
	std::cout << "press SPACE to PAUSE" << std::endl;
	std::cout << "press ESCAPE to return to MAIN MENU" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;
}

void PlayState::free()
{
	std::cout << "PlayState clean Successful" << std::endl;
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

	if (sys_input.CheckTriggeredInput(PE_N))
	{
		std::cout << "YOU ARE IN THE PLAY STATE" << std::endl;
	}

	if (sys_input.CheckTriggeredInput(PE_SPACE))
	{
		std::cout << "PAUSING....." << std::endl;
		game->PushState(PauseState::Instance());
	}

	if (sys_input.CheckTriggeredInput(PE_ESCAPE))
	{
		std::cout << "GOING BACK TO MAIN MENU" << std::endl;
		game->ChangeState(MenuState::Instance());
	}
}

void PlayState::draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}



