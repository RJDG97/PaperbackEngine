#include "Components/Visibility.h"

Visibility::Visibility() : b_detected_{false},
						   counter_{}
{}

void Visibility::Init() {
	// Create the map afterwards
	//GAMELOGIC->Visibilities[Component::GetOwner()->GetID()] = *this;
}

//void Visibility::Serialize(ISerializer& str) {
//	
//}