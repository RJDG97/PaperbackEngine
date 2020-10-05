#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameStates/GameState.h"

// inherits the abstract class GameState
class PlayState : public GameState
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
	void StateInputHandler(unsigned char key_val, Game* game);

/******************************************************************************/
/*!
  \fn PlayState()

  \brief A constructor for PlayState
*/
/******************************************************************************/
	PlayState() {}
};

extern PlayState m_PlayState;

#endif /*PLAYSTATE_H*/

