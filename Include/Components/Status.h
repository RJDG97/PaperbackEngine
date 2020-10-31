#ifndef _STATUS_H_
#define _STATUS_H_

#include "windows.h"
#include "Entity/Entity.h"

enum class StatusType {
	NONE = 0,
	INVISIBLE,
	HIT,
	BURROW
};

class Status : public Component {
	// Unique status of the entity
	StatusType status_;
	// Unique status timer
    float status_timer_;
	float cooldown_timer_;
public:
	friend class Collision;
	friend class Physics;
	friend class MenuState;
	friend class PlayState;
	friend class PauseState;

/******************************************************************************/
/*!
  \fn Status()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
	Status();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
  \fn Serialize

  \brief Retrieves the data from the stringstream to initialize data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

	~Status();
};

#endif