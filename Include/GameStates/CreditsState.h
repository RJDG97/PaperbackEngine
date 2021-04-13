#pragma once
/**********************************************************************************
*\file         CreditsState.h
*\brief        Contains declaration of CreditsState State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _CREDITS_STATE_H
#define _CREDITS_STATE_H

#include "GameStates/GameState.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"


class CreditsState : public GameState
{
	bool exiting_;
	ComponentManager* component_mgr_;
	EntityManager* entity_mgr_;
	const float set_time_{ 4.5f };
	float timer_;

public:
	friend class Game;

	/******************************************************************************/
	/*!
	  \fn CreditsState()

	  \brief A constructor for CutSceneState
	*/
	/******************************************************************************/
	CreditsState();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Creates entities that are to appear in the current state
	*/
	/******************************************************************************/
	void Init(std::string) override;

	/******************************************************************************/
	/*!
	  \fn Free()

	  \brief Releases all entities created within the current state
	*/
	/******************************************************************************/
	void Free() override;

	/******************************************************************************/
	/*!
	  \fn Update()

	  \brief Handles the updating of game logic relevant components like status
	*/
	/******************************************************************************/
	void Update(Game* game, float frametime) override;

	/******************************************************************************/
	/*!
	  \fn Draw()

	  \brief Handles drawing of entities that are currently active
	*/
	/******************************************************************************/
	void Draw(Game* game) override;

	/******************************************************************************/
	/*!
	  \fn StateInputHandler()

	  \brief Performs handling of inputs such as mouse clicks onto buttons
	*/
	/******************************************************************************/
	void StateInputHandler(Message* msg, Game* game) override;

	/******************************************************************************/
	/*!
	  \fn GetStateName()

	  \brief Returns the name of the current state
	*/
	/******************************************************************************/
	std::string GetStateName() { return "CreditsState"; }
};

extern CreditsState m_CreditsState;

#endif

