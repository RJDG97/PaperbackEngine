#ifndef _AI_H_
#define _AI_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include <iostream>
#include <sstream>
#include <vector>

class AI : public Component {

public:
	enum class AIType
	{
		StagBeetle,
		Mite,
		Hornet
	};

	enum AIState
	{
		Patrol,
		Chase,
		Attack,
		Return
	};

	friend class LogicSystem;
	friend class AIManager;

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

	AIState GetState();
		
	void SetState(AIState state);

private:

	AIType type_;
	int range_;
	int attackpower_;
	AIState state_;



};

#endif