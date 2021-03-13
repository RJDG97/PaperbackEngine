/**********************************************************************************
*\file         MenuState.h
*\brief        Contains definition of Menu State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameStates/GameState.h"
#include "Manager/LogicManager.h"

// Menu State Test
class MenuState : public GameState
{

	using LogicType = CMap<LogicComponent>;
	using LogicIt = LogicType::MapTypeIt;
	bool help_{ false };

	LogicType* logic_arr_;
	
	ComponentManager* component_mgr_;
	LogicManager* logic_mgr_;
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
  \fn MenuState()

  \brief A constructor for MenuState
*/
/******************************************************************************/
	MenuState() {}
};

extern MenuState m_MenuState;

#endif /*MENUSTATE_H*/

