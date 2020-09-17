#include <iostream>

#include "Game.h"
#include "GameState.h"
#include "InputSystem.h"
#include "TestFiles/Core.h"
#include "MenuState.h"

	Game::Game()
	{}

	void Game::init()
	{
		m_bRunning = true;
		std::cout << "Game Initialized Successfully" << std::endl;
	}

	// takes a pointer to a gamestate
	void Game::ChangeState(GameState* state)
	{
		// cleanup the current state
		if (!states.empty())
		{
			// free(); removes the old state from the stack
			states.back()->free(); 
			states.pop_back();
		}

		// store and init the new state
		states.push_back(state);
		states.back()->init();
		// back(); refers to the last element on the stack
	}

	// temporarily pushes a new state onto the stack
	void Game::PushState(GameState* state) // need to check if current state already exists in stack
	{

		// pause the current state
		if (!states.empty())
		{
			states.back()->pause();
		}

		//store and init the new state
		states.push_back(state);
		states.back()->init();
		std::cout << "States in stack after push: " << states.size();
	}
	void Game::PopState()
	{
		// cleanup the current state
		if (!states.empty())
		{
			states.back()->free();
			states.pop_back();
		}

		//resume the previous state
		if (!states.empty())
		{
			states.back()->resume();
		}

		std::cout << "States in stack after pop: " << states.size();
	}

	void Game::update(float frametime)
	{
		// let the current state take control
		if (!states.empty()) {
		
			states.back()->update(this);
		}

		// Swap to event systems
		if (sys_input.CheckTriggeredInput(0x34)) { // Number 4
			Message_CustomState msg{ &m_MenuState, MessageIDTypes::GSM_ChangeState }; // pass in another existing state?
			CORE->BroadcastMessage(&msg);
		}

		if (sys_input.CheckTriggeredInput(0x35)) {
			Message_CustomState msg{ &m_MenuState, MessageIDTypes::GSM_PushState }; // push maybe game state
			CORE->BroadcastMessage(&msg);
		}

		if (sys_input.CheckTriggeredInput(0x36)) {
			Message msg{ MessageIDTypes::GSM_PopState };
			CORE->BroadcastMessage(&msg);
		}

		/*if (sys_input.CheckTriggeredInput(0x34)) {
			Message msg{ MessageIDTypes::GSM_PauseState };
			CORE->BroadcastMessage(&msg);
		}*/
	}
	void Game::draw()
	{
		// let the current state take control
		states.back()->draw(this);
	}

	void Game::free()
	{
		while (!states.empty())
		{
			states.back()->free();
			states.pop_back();
		}
	}

	void Game::SendMessageD(Message* m) {

		std::cout << "Message received by Game" << std::endl;

		if (m->MessageID == MessageIDTypes::GSM_PushState) {

			Message_CustomState* msg = dynamic_cast<Message_CustomState*>(m);

			PushState(msg->_state);
		}

		if (m->MessageID == MessageIDTypes::GSM_ChangeState) {

			Message_CustomState* msg = dynamic_cast<Message_CustomState*>(m);

			ChangeState(msg->_state);
		}

		if (m->MessageID == MessageIDTypes::GSM_PauseState) {


		}

		if (m->MessageID == MessageIDTypes::GSM_PopState) {

			PopState();
		}
	}