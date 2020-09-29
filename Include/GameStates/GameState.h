#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Systems/Game.h"

// abstract/interface class for all the game states/scenes
class GameState
{
public:
	// virtual functions will get overriden in the different states

	//initialize the state
	virtual void init() = 0;

	//clean up the state
	virtual void free() = 0;

	// update the state according the player input
	// Game* is there just in case any of Game functions
	virtual void update(Game* game) = 0;

	// draw onto the console
	virtual void draw(Game* game) = 0;

	//virtual void load() = 0;
	//virtual void unload() = 0;

	// Pause current state
	virtual void pause() = 0;

	// Resume current state
	virtual void resume() = 0;

	virtual void StateInputHandler(unsigned char key_val) = 0;

	// change to the next game state
	void ChangeState(Game* game, GameState* state)
	{
		game->ChangeState(state);
	}

protected:
	// to implement the class as a singleton
	// ensuring there is only one instance of an object
	GameState() {}
};

#endif /*GAMESTATE_H*/
