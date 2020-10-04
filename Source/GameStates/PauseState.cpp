
#include <iostream>

#include "Systems/Game.h"
#include "GameStates/PauseState.h"
#include "Systems/InputSystem.h"

// SAMPLE PAUSE STATE

PauseState PauseState::m_PauseState;

void PauseState::init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "PauseState init Successful" << std::endl;
	std::cout << "press SPACE to RESUME" << std::endl << std::endl;	
	std::cout << "-----------------------------" << std::endl << std::endl;
}

void PauseState::free()
{
	std::cout << "PauseState clean Successful" << std::endl;
}

void PauseState::pause()
{
	std::cout << "PauseState Paused" << std::endl;
}

void PauseState::resume()
{
	std::cout << "PauseState Resumed" << std::endl;
}

void PauseState::update(Game* game)
{
	UNREFERENCED_PARAMETER(game);
	if (CORE->GetSystem<InputSystem>()->IsKeyTriggered(GLFW_KEY_B))
	{
		std::cout << "THE GAME IS PAUSED" << std::endl;
	}
	if (CORE->GetSystem<InputSystem>()->IsKeyTriggered(GLFW_KEY_SPACE))
	{
		std::cout << "RESUMING...." << std::endl;
		game->PopState();
	}
}

void PauseState::draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}



void PauseState::StateInputHandler(unsigned char key_val) {
	(void)key_val;

}