#include <iostream>

#include "Game.h"
#include "GameState.h"

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
	void Game::PushState(GameState* state)
	{
		// pause the current state
		if (!states.empty())
		{
			states.back()->pause();
		}

		//store and init the new state
		states.push_back(state);
		states.back()->init();
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
	}

	void Game::update()
	{
		// let the current state take control
		states.back()->update(this);
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