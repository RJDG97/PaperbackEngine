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


#ifndef _STOP_ANIMATION_H_
#define _STOP_ANIMATION_H_

#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include <sstream>
#include <memory>

class StopAnimation : public Component {

	std::string name_;

public:

	/******************************************************************************/
	/*!
	  \fn StopAnimation()

	  \brief Constructor for StopAnimation
	*/
	/******************************************************************************/
	StopAnimation();

	/******************************************************************************/
	/*!
	  \fn ~StopAnimation()

	  \brief Destructor for StopAnimation
	*/
	/******************************************************************************/
	~StopAnimation();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initialize the StopAnimation component
	*/
	/******************************************************************************/
	void Init();

	/******************************************************************************/
	/*!
	  \fn DeSerialize()

	  \brief Reads data from a stringstream and stores them into the data members
	*/
	/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

	/******************************************************************************/
	/*!
	  \fn Serialize()

	  \brief Write a component into JSON format
	*/
	/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

	/******************************************************************************/
	/*!
	  \fn Clone()

	  \brief Clones the existing component
	*/
	/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

	/******************************************************************************/
	/*!
	  \fn Name()

	  \brief Get the name of the animation that is to be stopped at
	*/
	/******************************************************************************/
	std::string Name() const;
};
#endif