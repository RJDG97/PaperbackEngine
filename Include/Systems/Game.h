/**********************************************************************************
*\file         Game.h
*\brief        Contains declaration of functions and variables used for
*			   the Game State Manager
*
*\author	   Sim Ee Ling, Adele, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef GAME_H
#define GAME_H

#include <vector>
#include <unordered_map>
#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Components/Status.h"
#include "Components/BasicAI.h"
#include "Components/InputController.h"
#include "Manager/ComponentManager.h"
#include <filesystem>

namespace fs = std::filesystem;

class GameState;

class Game : public ISystem
{
public:
	friend class MenuState;
	friend class PlayState;
	friend class EditorState;


	using StatusMapType = CMap<Status>;
	using StatusIt = StatusMapType::MapTypeIt;
	using BasicAIType = CMap<BasicAI>;
	using BasicAIIt = BasicAIType::MapTypeIt;
	using InputControllerType = CMap<InputController>;
	using InputControllerIt = InputControllerType::MapTypeIt;


/******************************************************************************/
/*!
  \fn Game()

  \brief Initializes current and maximum health of the entity
*/
/******************************************************************************/
	Game();

/******************************************************************************/
/*!
  \fn Draw()

  \brief Draw all active entities within the state
*/
/******************************************************************************/
	void Draw();

/******************************************************************************/
/*!
  \fn Free()

  \brief Clean up the current state
*/
/******************************************************************************/
	void Free();

/******************************************************************************/
/*!
  \fn ChangeState()

  \brief Change current state to new state
*/
/******************************************************************************/
	void ChangeState(GameState* state, std::string level_name = {});

/******************************************************************************/
/*!
  \fn PushState()

  \brief Add new state onto stack
*/
/******************************************************************************/
	void PushState(GameState* state);

/******************************************************************************/
/*!
  \fn PopState()

  \brief Remove current state from stack
*/
/******************************************************************************/
	void PopState();

/******************************************************************************/
/*!
  \fn Running()

  \brief Returns current status of state
*/
/******************************************************************************/
	bool Running()
	{
		return b_running_;
	}

/******************************************************************************/
/*!
  \fn Quit()

  \brief Sets current status of state to quit
*/
/******************************************************************************/
	void Quit()
	{
		b_running_ = false;
	}

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the current state
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Updating the current state based on player input
*/
/******************************************************************************/
	void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() override { return "Game State Manager"; }

/******************************************************************************/
/*!
  \fn GetStateName()

  \brief Returns the name of the current state
*/
/******************************************************************************/
	std::string GetStateName();

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages sent by the Core Engine
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m) override;

	std::vector<std::string> LoadAllTextureJson();

private:
	bool debug_;

	// stack to hold the states
	std::vector<GameState*> states_;
	std::vector<std::string> files_to_load_;
	// for the game loop
	bool b_running_;

	//using StatusMapType = CMap<Status>;
	//using StatusIt = StatusMapType::MapTypeIt;
	StatusMapType* status_arr_;

	//using BasicAIType = CMap<BasicAI>;
	//using BasicAIIt = BasicAIType::MapTypeIt;
	BasicAIType* basicai_arr_;

	//using InputControllerType = CMap<InputController>;
	//using InputControllerIt = InputControllerType::MapTypeIt;
	InputControllerType* input_controller_arr_;


/******************************************************************************/
/*!
  \fn CheckExist()

  \brief Checks whether the state already exists on the stack
*/
/******************************************************************************/
	bool CheckExist(GameState* compare);
};

#endif /*GAME_H*/