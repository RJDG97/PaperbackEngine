#include "Components/Status.h"
#include "Engine/Core.h"
#include "Systems/Game.h"

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

	CORE->GetSystem<Game>()->AddStatusComponent(Component::GetOwner()->GetID(), this);
}

/******************************************************************************/
/*!
  \fn Serialize

  \brief Retrieves the data from the stringstream to initialize data members
*/
/******************************************************************************/
void Status::Serialize(std::stringstream& data) {
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

	CORE->GetSystem<Game>()->RemoveStatusComponent(Component::GetOwner()->GetID());
}