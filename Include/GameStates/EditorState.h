/**********************************************************************************
*\file         EditorState.h
*\brief        Contains declaration of Editor State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "GameStates/GameState.h"
#include "Manager/EntityManager.h"
#include "Systems/ImguiSystem.h"

// inherits the abstract class GameState
class EditorState : public GameState
{
	bool pause_;
	ImguiSystem* imgui_;

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
	EditorState() {}
};
extern EditorState m_EditorState;

#endif /*PLAYSTATE_H*/

