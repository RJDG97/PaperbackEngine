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
	friend class Physics; // Temporary friend class for testing purposes

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
  \fn Init()

  \brief Logic where the component will add itself to a system's map will
		 be placed here
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void Serialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn SerializeClone()

  \brief Serializes data members within the Component
*/
/******************************************************************************/
	void SerializeClone(std::stringstream& data);

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;
};

#endif