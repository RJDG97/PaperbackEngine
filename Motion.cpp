#include <iostream>
#include "Motion.h"
#include "Physics.h"

Motion::Motion() : velocity_{},
				   acceleration_{}
{}

Motion::~Motion() {

	PHYSICS->RemoveMotionComponent(Component::GetOwner()->GetID());

}

void Motion::Init() {
	// Create the map afterwards
	//PHYSICS->Motions[Component::GetOwner()->GetID()] = *this;
	PHYSICS->AddMotionComponent(Component::GetOwner()->GetID(), this);
}

void Motion::PublishResults() {
	//transform->_position = position;
}

void Motion::Serialize(std::stringstream& data) {
	std::cout << "Entered Serialize Motion w/ stream" << std::endl;
	
	data >> velocity_.x >> velocity_.y >> acceleration_.x >> acceleration_.y;

	std::cout << "Velocity read: " << velocity_.x << ", " << velocity_.y << std::endl;
	std::cout << "Accleration read: " << acceleration_.x << ", " << acceleration_.y << std::endl;
}