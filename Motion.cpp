#include "Motion.h"
#include "Physics.h"

Motion::Motion() : velocity_{},
				   acceleration_{}
{}

void Motion::Init() {
	// Create the map afterwards
	//PHYSICS->Motions[Component::GetOwner()->GetID()] = *this;
	PHYSICS->AddMotionComponent(Component::GetOwner()->GetID(), this);
}

void Motion::PublishResults() {
	//transform->_position = position;
}

//void Motion::Serialize(ISerializer& str) {
//	
//}