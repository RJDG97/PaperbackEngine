#ifndef _AI_H_
#define _AI_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
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
	int GetRange();

	/******************************************************************************/
	/*!
	  \fn SetRange(int range)

	  \brief Set component range
	*/
	/******************************************************************************/
	void SetRange(int range);

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
	std::vector<Vector2D> GetDestinations();

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

private:

	AIType type_;
	AIState state_;
	int range_;
	int attackpower_;
	float speed_;
	size_t num_destinations_;
	std::vector<Vector2D> destinations_;
	DestinationIt current_destination_;

};

#endif