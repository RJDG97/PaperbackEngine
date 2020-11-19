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