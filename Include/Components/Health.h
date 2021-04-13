/**********************************************************************************
*\file         Health.h
*\brief        Contains declaration of functions and variables used for
*			   the Health Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _HEALTH_H_
#define _HEALTH_H_

#include "IComponent.h"
#include <list>

// Contains data about the health of an entity that the component will be attached to
class Health : public Component 
{
	int current_health_;
	int maximum_health_;
public:
	friend class Physics;
	friend class Collision;

/******************************************************************************/
/*!
  \fn Health()

  \brief Constructor for Health that defaults the current and maximum health 
		 of the entity
*/
/******************************************************************************/
	Health();

/******************************************************************************/
/*!
  \fn ~Health()

  \brief Destructor for Health that removes the component from the component map
*/
/******************************************************************************/
	~Health();

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
  \fn GetCurrentHealth()

  \brief Retrieve the current health of the entity
*/
/******************************************************************************/
	int GetCurrentHealth();

/******************************************************************************/
/*!
  \fn SetCurrentHealth(int newHealth)

  \brief Set the current health of the entity
*/
/******************************************************************************/
	void SetCurrentHealth(int newHealth);

/******************************************************************************/
/*!
  \fn GetMaxHealth()

  \brief Retrieve the Max health of the entity
*/
/******************************************************************************/
	int GetMaxHealth();

/******************************************************************************/
/*!
  \fn SetMaxHealth(int newHealth)

  \brief Sets the Max health of the entity
*/
/******************************************************************************/
	void SetMaxHealth(int newHealth);

/******************************************************************************/
/*!
  \fn IncrementHealth()

  \brief Increments health of entity if not max
*/
/******************************************************************************/
	void IncrementHealth();
};

#endif