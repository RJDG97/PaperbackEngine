#pragma once
#ifndef _SPLASH_STATE_H
#define _SPLASH_STATE_H

#include "GameStates/GameState.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"


class SplashState : public GameState
{
	ComponentManager* component_mgr_;
	EntityManager* entity_mgr_;

public:
	friend class Game;

	/******************************************************************************/
	/*!
	  \fn SplashState()

	  \brief A constructor for SplashState
	*/
	/******************************************************************************/
	SplashState();

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
	std::string GetStateName() { return "SplashState"; }
};

extern SplashState m_SplashState;

#endif /*PLAYSTATE_H*/

