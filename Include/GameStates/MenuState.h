#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameStates/GameState.h"

// Menu State Test
class MenuState : public GameState
{
public:
	friend class Game;

/******************************************************************************/
/*!
  \fn Init()

  \brief Creates entities that are to appear in the current state
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Free()

  \brief Releases all entities created within the current state
*/
/******************************************************************************/
	void Free();

/******************************************************************************/
/*!
  \fn Update()

  \brief Handles the updating of game logic relevant components like status
*/
/******************************************************************************/
	void Update(Game* game, float frametime);

/******************************************************************************/
/*!
  \fn Draw()

  \brief Handles drawing of entities that are currently active
*/
/******************************************************************************/
	void Draw(Game* game);

/******************************************************************************/
/*!
  \fn StateInputHandler()

  \brief Performs handling of inputs such as mouse clicks onto buttons
*/
/******************************************************************************/
	void StateInputHandler(Message* msg, Game* game);

/******************************************************************************/
/*!
  \fn GetStateName()

  \brief Returns the name of the current state
*/
/******************************************************************************/
	std::string GetStateName() override;

/******************************************************************************/
/*!
  \fn MenuState()

  \brief A constructor for MenuState
*/
/******************************************************************************/
	MenuState() {}
};

extern MenuState m_MenuState;

#endif /*MENUSTATE_H*/

