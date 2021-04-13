/**********************************************************************************
*\file         Status.cpp
*\brief        Contains definition of functions and variables used for
*			   the Status Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Status.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Systems/Game.h"
#include "Systems/Collision.h"
#include "Systems/Physics.h"

/******************************************************************************/
/*!
  \fn Status()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
Status::Status() : 
	status_{ StatusType::NONE },
	status_timer_{},
    cooldown_timer_{}
{}

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialize the component attached to the entity during compile time
*/
/******************************************************************************/
void Status::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Status>(GetOwner()->GetID(), this);
}

void Status::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Status");

	writer->EndObject();
}

/******************************************************************************/
/*!
  \fn DeSerialize

  \brief Retrieves the data from the stringstream to initialize data members
*/
/******************************************************************************/
void Status::DeSerialize(std::stringstream& data) {

	(void)data;
}

std::shared_ptr<Component> Status::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Status Component\n");

	std::shared_ptr<Status> cloned = std::make_shared<Status>();

	cloned->status_ = status_;
	cloned->status_timer_ = status_timer_;
	cloned->cooldown_timer_ = cooldown_timer_;

	return cloned;
}

Status::~Status() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Status>(Component::GetOwner()->GetID());
}

StatusType Status::GetStatus() {

	return status_;
}

void Status::SetStatus(StatusType newStatus) {

	status_ = newStatus;
}

float Status::GetStatusTimer() {

	return status_timer_;
}

void Status::SetStatusTimer(float newtimer) {

	status_timer_ = newtimer;
}

float Status::GetCooldownTimer() {

	return cooldown_timer_;
}

void Status::SetCoolDownTimer(float newtimer) {

	cooldown_timer_ = newtimer;
}
