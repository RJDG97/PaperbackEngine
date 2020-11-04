#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameStates/GameState.h"
#include "Manager/EntityManager.h"
#include "Components/Scale.h"

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
	void Init(std::string);

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
  \fn SetStatus()

  \brief Helper function for StateInputHandler that sets the Status components
		 of a specific entity type to that of a new status
*/
/******************************************************************************/
	void SetStatus(std::string entity_name, StatusType status_type, float status_length, Game* game);

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
  \fn PlayState()

  \brief A constructor for PlayState
*/
/******************************************************************************/
	PlayState() {}
};

extern PlayState m_PlayState;

#endif /*PLAYSTATE_H*/

