#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "GameStates/GameState.h"

class PauseState: public GameState 
{
public:

	void init();
	void free();

	void update(Game* game);
	void draw(Game* game);

	void pause();
	void resume();

	void StateInputHandler(unsigned char key_val);

	// Implement Singleton Pattern
	// Returns a pointer to a static instance of the class
	static PauseState* Instance()
	{
		return &m_PauseState;
	}

protected:
	PauseState() {}

private:

	static PauseState m_PauseState;
};

#endif /*PAUSESTATE_H*/

