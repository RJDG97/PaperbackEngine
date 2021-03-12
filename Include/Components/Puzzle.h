/**********************************************************************************
*\file         Puzzle.h
*\brief        Contains declaration of functions and variables used for
*			   the Puzzle Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _PUZZLE_H_
#define _PUZZLE_H_

#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class Puzzle : public Component {

	size_t current_;
	size_t requirement_;

public:

	friend class PuzzleSystem;

	/******************************************************************************/
	/*!
	  \fn Puzzle()

	  \brief Constructor for Puzzle
	*/
	/******************************************************************************/
	Puzzle();

	/******************************************************************************/
	/*!
	  \fn ~Puzzle()

	  \brief Destructor for Puzzle
	*/
	/******************************************************************************/
	~Puzzle();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initialize the Puzzle component
	*/
	/******************************************************************************/
	void Init();

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Write a component into JSON format
	*/
	/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

	/******************************************************************************/
	/*!
	  \fn SerializeClone()

	  \brief Write a component into JSON format
	*/
	/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

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

	  \brief Reads data from a stringstream and stores them into the data members
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
};
#endif