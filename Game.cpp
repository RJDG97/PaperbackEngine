#include <iostream>

#include "Game.h"
#include "GameState.h"
#include "InputSystem.h"
#include "Core.h"
#include "MenuState.h"
#include <algorithm>

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

		auto CheckExist = [state](GameState* s) {
			if (s == state) {
				std::cout << "Error: State already exists on stack" << std::endl;
				return true;
			}
			return false;
		};

		//store and init the new state
		if (std::find_if(states.begin(), states.end(), CheckExist) == states.end()) {
			states.push_back(state);
			states.back()->init();
			std::cout << "States in stack after push: " << states.size() << std::endl;
		}
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

		std::cout << "States in stack after pop: " << states.size() << std::endl;
	}

	void Game::update(float frametime)
	{
		// let the current state take control
		if (!states.empty()) {
		
			states.back()->update(this);
		}
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

		switch (m->MessageID) {

		case MessageIDTypes::GSM_PushState:
		{
			Message_CustomState* msg = dynamic_cast<Message_CustomState*>(m);
			PushState(msg->_state);
			break;
		}
		case MessageIDTypes::GSM_ChangeState:
		{	
			Message_CustomState* msg = dynamic_cast<Message_CustomState*>(m);
			ChangeState(msg->_state);
			break;
		}
		case MessageIDTypes::GSM_PauseState:
		{
			break;
		}
		case MessageIDTypes::GSM_PopState:
		{
			PopState();
			break;
		}
		default:
			break;
		}
	}