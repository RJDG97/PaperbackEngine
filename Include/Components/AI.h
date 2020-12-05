/**********************************************************************************
*\file         AI.h
*\brief        Contains declaration of functions and variables used for
*			   the AI Component
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _AI_H_
#define _AI_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include "Systems/FrameRateController.h"
#include <iostream>
#include <sstream>
#include <vector>

using DestinationIt = std::vector<Vector2D>::iterator;

class AI : public Component {

public:

	enum AIType
	{
		StagBeetle,
		Mite,
		Hornet
	};

	enum AIState
	{
		Patrol,
		Detected,
		Chase,
		Attack,
		Withdraw,
		Return
	};

	friend class LogicSystem;

	/******************************************************************************/
	/*!
	  \fn BasicAI()

	  \brief Constructor for BasicAI that defaults the data members of the
			 component
	*/
	/******************************************************************************/
	AI();

	/******************************************************************************/
	/*!
	  \fn ~BasicAI()

	  \brief Destructor for BasicAI that removes the component from the
			 Game system AI component map
	*/
	/******************************************************************************/
	~AI();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Adds the component itself to the Game system AI map
	*/
	/******************************************************************************/
	void Init();

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Serialises a component into JSON format
	*/
	/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

	/******************************************************************************/
	/*!
	  \fn SerializeClone()

	  \brief Serialises a cloned component into JSON format
	*/
	/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

	/******************************************************************************/
	/*!
	  \fn DeSerialize()

	  \brief Reads data from a stringstream and stores them into the data members
	*/
	/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

	/******************************************************************************/
	/*!
	  \fn DeSerializeClone()

	  \brief Retrieves data that will initialise data that can be unique from other
			 components
	*/
	/******************************************************************************/
	void DeSerializeClone(std::stringstream& data) override;

	/******************************************************************************/
	/*!
	  \fn Clone()

	  \brief Clones the existing component
	*/
	/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

	/******************************************************************************/
	/*!
	  \fn GetState()

	  \brief Get component state
	*/
	/******************************************************************************/
	AIState GetState();
	
	/******************************************************************************/
	/*!
	  \fn SetState(AIState state)

	  \brief Set component state
	*/
	/******************************************************************************/
	void SetState(AIState state);

	/******************************************************************************/
	/*!
	  \fn GetRange()

	  \brief Get component range
	*/
	/******************************************************************************/
	float GetRange();

	/******************************************************************************/
	/*!
	  \fn SetRange(int range)

	  \brief Set component range
	*/
	/******************************************************************************/
	void SetRange(float range);

	/******************************************************************************/
	/*!
	  \fn GetAtk()

	  \brief Get component attack
	*/
	/******************************************************************************/
	int GetAtk();

	/******************************************************************************/
	/*!
	  \fn SetAtk(int atk)

	  \brief Set component attack
	*/
	/******************************************************************************/
	void SetAtk(int atk);

	/******************************************************************************/
	/*!
	  \fn GetSpeed()

	  \brief Get component speed
	*/
	/******************************************************************************/
	float GetSpeed();

	/******************************************************************************/
	/*!
	  \fn SetSpeed(float speed)

	  \brief Set component speed
	*/
	/******************************************************************************/
	void SetSpeed(float speed);

	/******************************************************************************/
	/*!
	  \fn GetNumDes()

	  \brief Get component number of destinations
	*/
	/******************************************************************************/
	size_t GetNumDes();

	/******************************************************************************/
	/*!
	  \fn SetNumDes(size_t numdes)

	  \brief Set component number of destinations
	*/
	/******************************************************************************/
	void SetNumDes(size_t numdes);

	/******************************************************************************/
	/*!
	  \fn GetDestinations()

	  \brief Get component destinations
	*/
	/******************************************************************************/
	std::vector<Vector2D>& GetDestinations();

	/******************************************************************************/
	/*!
	  \fn SetDestinations(std::vector<Vector2D> des)

	  \brief Set component destinations
	*/
	/******************************************************************************/
	void SetDestinations(std::vector<Vector2D> des);

	/******************************************************************************/
	/*!
	  \fn GetCurrentDes()

	  \brief Get component current destinations
	*/
	/******************************************************************************/
	DestinationIt GetCurrentDes();

	/******************************************************************************/
	/*!
	  \fn SetCurrentDes(DestinationIt Cdes

	  \brief Set component current destinations
	*/
	/******************************************************************************/
	void SetCurrentDes(DestinationIt Cdes);

	/******************************************************************************/
	/*!
	  \fn AIType GetType()

	  \brief Get enemy type
	*/
	/******************************************************************************/
	AIType GetType();

	/******************************************************************************/
	/*!
	  \fn GetPlayerLastPos()

	  \brief Get players last position
	*/
	/******************************************************************************/
	Vector2D GetPlayerLastPos();

	/******************************************************************************/
	/*!
	  \fn SetPlayerLastPos(Vector2D pos)

	  \brief Set player last position
	*/
	/******************************************************************************/
	void SetPlayerLastPos(Vector2D pos);

	/******************************************************************************/
	/*!
	  \fn GetNewDestination()

	  \brief Get new destinations
	*/
	/******************************************************************************/
	Vector2D GetNewDestination();

	/******************************************************************************/
	/*!
	  \fn SetNewDestination(Vector2D newnode)

	  \brief Set new destinations
	*/
	/******************************************************************************/
	void SetNewDestination(Vector2D newnode);

	/******************************************************************************/
	/*!
	  \fn GetPath()

	  \brief Get new path
	*/
	/******************************************************************************/
	std::vector<Vector2D>& GetPath();

	/******************************************************************************/
	/*!
	  \fn SetPath()

	  \brief Set new path
	*/
	/******************************************************************************/
	void SetPath(std::vector<Vector2D>& path);

	/******************************************************************************/
	/*!
	  \fn GetTimer()

	  \brief Gets timer
	*/
	/******************************************************************************/
	Time_Channel& GetTimer();

	/******************************************************************************/
	/*!
	  \fn GetLife()

	  \brief Returns life status
	*/
	/******************************************************************************/
	bool GetLife();

	/******************************************************************************/
	/*!
	  \fn SetLife()

	  \brief Set life's status
	*/
	/******************************************************************************/
	void SetLife(bool life);

private:

	AIType type_;
	AIState state_;
	float range_;
	int attackpower_;
	float speed_;
	size_t num_destinations_;
	Vector2D new_node_;
	std::vector<Vector2D> destinations_;
	DestinationIt current_destination_;
	Vector2D player_last_pos_;
	std::vector<Vector2D> path_;
	Time_Channel recovery_timer_;
	bool alive_;
};

#endif