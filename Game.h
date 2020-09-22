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
	void Draw();
	void Free();

	// functions for changing the game state
	void ChangeState(GameState* state);

	// add a state onto the stack
	void PushState(GameState* state);

	// remove the state (on the top of the stack)
	void PopState();

	bool Running() // getter function
	{
		return b_running_;
	}

	void Quit() // setter function
	{
		b_running_ = false;
	}

	void Init() override;
	void Update(float frametime) override;
	virtual std::string GetName() override { return "Game State Manager"; }
	virtual void SendMessageD(Message* m) override;

private:
	// stack to hold the states
	std::vector<GameState*> states_;

	// for the game loop
	bool b_running_;
};

#endif /*GAME_H*/