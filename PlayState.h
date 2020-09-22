#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"

// inherits the abstract class GameState
class PlayState : public GameState
{
public:

	void init();
	void free();

	void update(Game* game);
	void draw(Game* game);

	void pause();
	void resume();

	void StateInputHandler(int key_val);
	// Implement Singleton Pattern
	// Returns a pointer to a static instance of the class
	//static PlayState* Instance()
	//{
	//	return &m_PlayState;
	//}

//protected:
	PlayState() {}

//private:
//	static PlayState m_PlayState;
};

extern PlayState m_PlayState;

#endif /*PLAYSTATE_H*/

