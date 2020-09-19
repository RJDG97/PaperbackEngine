#include "Motion.h"

Motion::Motion() : velocity_{},
				   acceleration_{}
{}

void Motion::init() {
	// Create the map afterwards
	//PHYSICS->Motions[Component::GetOwner()->GetID()] = *this;
}

void Motion::PublishResults() {
	//transform->_position = position;
}

//void Motion::serialize(ISerializer& str) {
//	
//}