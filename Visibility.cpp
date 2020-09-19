#include "Visibility.h"

Visibility::Visibility() : b_detected_{false},
						   counter_{}
{}

void Visibility::init() {
	// Create the map afterwards
	//GAMELOGIC->Visibilities[Component::GetOwner()->GetID()] = *this;
}

void Visibility::PublishResults() {
	//transform->_position = position;
}

//void Visibility::serialize(ISerializer& str) {
//	
//}