#ifndef GAME_H
#define GAME_H

#include <vector>
#include "ISystem.h"
#include "Message.h"

class GameState;

class Game : public ISystem
{
public:
	Game();

	// state functions
	//void init();
	//void update();
	void draw();
	void free();

	// functions for changing the game state
	void ChangeState(GameState* state);

	// add a state onto the stack
	void PushState(GameState* state);

	// remove the state (on the top of the stack)
	void PopState();

	bool Running() // getter function
	{
		return m_bRunning;
	}

	void Quit() // setter function
	{
		m_bRunning = false;
	}

	void init();
	void update(float frametime);
	virtual std::string GetName() { return "Game State Manager"; }
	virtual void SendMessageD(Message* m);

private:
	// stack to hold the states
	std::vector<GameState*> states;

	// for the game loop
	bool m_bRunning;
};

#endif /*GAME_H*/

