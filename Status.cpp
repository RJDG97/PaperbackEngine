#include "Status.h"

Status::Status() : b_player_hit_{false},
				   counter_{}
{}

void Status::init() {
	// Create the map afterwards
	//GAMELOGIC->Visibilities[Component::GetOwner()->GetID()] = *this;
}

void Status::PublishResults() {
	//transform->_position = position;
}

//void Visibility::serialize(ISerializer& str) {
//	
//}