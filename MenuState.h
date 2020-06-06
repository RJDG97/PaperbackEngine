#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"

// Menu State Test
class MenuState : public GameState
{
public:

	void init();
	void free();

	void update(Game* game);
	void draw(Game* game);

	void pause();
	void resume();

	// Implement Singleton Pattern
	// Returns a pointer to a static instance of the class
	static MenuState* Instance()
	{
		return &m_MenuState;
	}

protected:

	MenuState() {}

private:

	static MenuState m_MenuState;
	
};

#endif /*MENUSTATE_H*/

