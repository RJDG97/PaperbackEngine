/**********************************************************************************
*\file         Attack.h
*\brief        Contains definition of functions and variables used for
*			   the Attack Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "Entity/Entity.h"

class Attack : public Component {
	float attack_power_;
public:

/******************************************************************************/
/*!
  \fn Attack()

  \brief Constructor for Attack, it defaults the values of its data members
*/
/******************************************************************************/
	Attack();

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
};

#endif