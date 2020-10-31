#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Systems/Game.h"
#include "Manager/ComponentManager.h"

// abstract/interface class for all the game states/scenes
class GameState
{
public:
	// virtual functions will get overriden in the different states

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the current state
*/
/******************************************************************************/
	virtual void Init() = 0;

/******************************************************************************/
/*!
  \fn Free()

  \brief Clean up the current state
*/
/******************************************************************************/
	virtual void Free() = 0;

/******************************************************************************/
/*!
  \fn Update()

  \brief Updating the current state based on player input
*/
/******************************************************************************/
	virtual void Update(Game* game, float frametime) = 0;

/******************************************************************************/
/*!
  \fn Draw()

  \brief Draw all active entities within the state
*/
/******************************************************************************/
	virtual void Draw(Game* game) = 0;

/******************************************************************************/
/*!
  \fn StateInputHandler()

  \brief Handles gameplay events from input system
*/
/******************************************************************************/
	virtual void StateInputHandler(Message* msg, Game* game = nullptr) = 0;

/******************************************************************************/
/*!
  \fn ChangeState()

  \brief Change current state to new state
*/
/******************************************************************************/
	void ChangeState(Game* game, GameState* state)
	{
		game->ChangeState(state);
	}

protected:
/******************************************************************************/
/*!
  \fn GameState()

  \brief Initializes the current state
*/
/******************************************************************************/
	GameState() {}
};

#endif /*GAMESTATE_H*/
