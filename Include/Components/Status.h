/**********************************************************************************
*\file         Status.h
*\brief        Contains declaration of functions and variables used for
*			   the Status Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
	friend class EditorState;

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

/******************************************************************************/
/*!
  \fn ~Status()

  \brief Handles the cleanup of the component
*/
/******************************************************************************/
	~Status();
/******************************************************************************/
/*!
  \fn GetStatus()

  \brief Retrieves the status of the entity
*/
/******************************************************************************/
	StatusType GetStatus();

/******************************************************************************/
/*!
  \fn SetStatus(StatusType newStatus)

  \brief Set the entity status
*/
/******************************************************************************/
	void SetStatus(StatusType newStatus);

/******************************************************************************/
/*!
  \fn GetStatusTimer()

  \brief retrieve the status timer
*/
/******************************************************************************/
	float GetStatusTimer();

/******************************************************************************/
/*!
  \fn SetStatusTimer(float newtimer)

  \brief set the status timer
*/
/******************************************************************************/
	void SetStatusTimer(float newtimer);

/******************************************************************************/
/*!
  \fn GetCooldownTimer()

  \brief Get the cooldown timer
*/
/******************************************************************************/
	float GetCooldownTimer();

/******************************************************************************/
/*!
  \fn SetCoolDownTimer(float newtimer)

  \brief Set the cooldown timer
/******************************************************************************/
	void SetCoolDownTimer(float newtimer);

};

#endif