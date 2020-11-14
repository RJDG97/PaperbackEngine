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

	//CORE->GetSystem<Game>()->AddStatusComponent(GetOwner()->GetID(), this);
	//CORE->GetSystem<Collision>()->AddStatusComponent(GetOwner()->GetID(), this);
	//CORE->GetSystem<Physics>()->AddStatusComponent(GetOwner()->GetID(), this);
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
    /*
    data >> status_;
    */
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

	//CORE->GetSystem<Game>()->RemoveStatusComponent(Component::GetOwner()->GetID());
	//CORE->GetSystem<Collision>()->RemoveStatusComponent(Component::GetOwner()->GetID());
	//CORE->GetSystem<Physics>()->RemoveStatusComponent(Component::GetOwner()->GetID());
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
