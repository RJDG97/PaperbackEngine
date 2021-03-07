/**********************************************************************************
*\file         WinLoseState.h
*\brief        Contains declaration of WinLose State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _WIN_LOSE_STATE_H_
#define _WIN_LOSE_STATE_H_

#include "GameStates/GameState.h"

class WinLoseState : public GameState
{
	
	bool is_win_ = false;

public:
	WinLoseState() = default;

/******************************************************************************/
/*!
  \fn Init()

  \brief Creates entities that are to appear in the current state
*/
/******************************************************************************/
	void Init(std::string level_name) override;

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

  \brief Get state's name
*/
/******************************************************************************/
	std::string GetStateName() override;
};

extern WinLoseState m_WinLoseState;

#endif