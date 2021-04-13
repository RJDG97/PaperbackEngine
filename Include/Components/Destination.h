/**********************************************************************************
*\file         Destination.h
*\brief        Contains declaration of functions and variables used for
*			   the Destination Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#include "MathLib/Vector2D.h"
#include "IComponent.h"

class Destination : public Component
{
	Vector2D destination_;
	bool ui_layer_;

public:

	/******************************************************************************/
	/*!
	  \fn Destination()

	  \brief Constructor for Destination component
	*/
	/******************************************************************************/
	Destination();

	/******************************************************************************/
	/*!
	  \fn ~Destination()

	  \brief Destructor for Destination component that removes the component from 
			 the component map
	*/
	/******************************************************************************/
	~Destination();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Logic where the component will add itself to a system's map will
			 be placed here
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
	  \fn Serialize()

	  \brief Serialises a component into JSON format
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

	  \brief Deserializes data members within the Component
	*/
	/******************************************************************************/
	void DeSerializeClone(std::stringstream& data);

	/******************************************************************************/
	/*!
	  \fn Clone()

	  \brief Clones the existing component
	*/
	/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

	/******************************************************************************/
	/*!
	  \fn GetDestination()

	  \brief Gets the destination of the entity
	*/
	/******************************************************************************/
	const Vector2D& GetDestination() const;
};

#endif