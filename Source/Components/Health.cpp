#include "Components/Health.h"
#include "Systems/Physics.h"
#include <iostream>

/******************************************************************************/
/*!
  \fn Health()

  \brief Initializes current and maximum health of the entity
*/
/******************************************************************************/
Health::Health() {
	current_health_ = maximum_health_ = 5;
}

/******************************************************************************/
/*!
  \fn Init()

  \brief Initializes current and maximum health of the entity
*/
/******************************************************************************/
void Health::Init() {

}

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Retrieves the data from the stringstream to initialize data members
*/
/******************************************************************************/
void Health::Serialize(std::stringstream& data) {

	data >> maximum_health_;

	current_health_ = maximum_health_;
}