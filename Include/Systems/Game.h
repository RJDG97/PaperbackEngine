#ifndef GAME_H
#define GAME_H

#include <vector>
#include <unordered_map>
#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Components/Status.h"
#include "Components/BasicAI.h"
#include "Components/InputController.h"

class GameState;

class Game : public ISystem
{
public:
	friend class MenuState;
	friend class PlayState;
	friend class PauseState;

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
	void ChangeState(GameState* state);

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
  \fn SendMessageD()

  \brief Receives messages sent by the Core Engine
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m) override;

/******************************************************************************/
/*!
  \fn AddStatusComponent()

  \brief Adds a newly created Status Component to the status component array
         within the Game System
*/
/******************************************************************************/
	void AddStatusComponent(EntityID id, Status* status);

/******************************************************************************/
/*!
  \fn RemoveStatusComponent()

  \brief Removes a Status Component from the status component array within the 
	     Game System
*/
/******************************************************************************/
	void RemoveStatusComponent(EntityID id);

/******************************************************************************/
/*!
  \fn AddBasicAIComponent()

  \brief Adds a newly created BasicAI Component to the BasicAI component array
		 within the Game System
*/
/******************************************************************************/
	void AddBasicAIComponent(EntityID id, BasicAI* basic_ai);

/******************************************************************************/
/*!
  \fn RemoveBasicAIComponent()

  \brief Removes a BasicAI Component from the BasicAI component array within the
		 Game System
*/
/******************************************************************************/
	void RemoveBasicAIComponent(EntityID id);

/******************************************************************************/
/*!
  \fn AddInputControllerComponent()

  \brief Adds a newly created InputController Component to the InputController 
		 component array within the Game System
*/
/******************************************************************************/
	void AddInputControllerComponent(EntityID id, InputController* input_controller);

/******************************************************************************/
/*!
  \fn RemoveInputControllerComponent()

  \brief Removes a InputController Component from the InputController 
		 component array within the Game System
*/
/******************************************************************************/
	void RemoveInputControllerComponent(EntityID id);

private:
	bool debug_;

	// stack to hold the states
	std::vector<GameState*> states_;

	// for the game loop
	bool b_running_;

	using StatusIt = std::unordered_map<EntityID, Status*>::iterator;
	std::unordered_map<EntityID, Status*> status_arr_;

	using BasicAIIt = std::unordered_map<EntityID, BasicAI*>::iterator;
	std::unordered_map<EntityID, BasicAI*> basicai_arr_;

	using InputControllerIt = std::unordered_map<EntityID, InputController*>::iterator;
	std::unordered_map < EntityID, InputController*> input_controller_arr_;

/******************************************************************************/
/*!
  \fn CheckExist()

  \brief Checks whether the state already exists on the stack
*/
/******************************************************************************/
	bool CheckExist(GameState* compare);
};

#endif /*GAME_H*/