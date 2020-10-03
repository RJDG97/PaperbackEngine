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

Status::~Status() {

	CORE->GetSystem<Game>()->RemoveStatusComponent(Component::GetOwner()->GetID());
}