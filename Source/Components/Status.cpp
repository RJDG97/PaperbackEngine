#include "Components/Status.h"

Status::Status() : b_player_hit_{false},
				   counter_{}
{}

void Status::Init() {
	// Create the map afterwards
	//GAMELOGIC->Visibilities[Component::GetOwner()->GetID()] = *this;
}

//void Visibility::Serialize(ISerializer& str) {
//	
//}