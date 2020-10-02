#include "Components/Attack.h"

Attack::Attack() : attack_power_{}
{}

void Attack::Init() {
	// Create the map afterwards
	//GAMELOGIC->Attacks[Component::GetOwner()->GetID()] = *this;
}

//void Attack::Serialize(ISerializer& str) {
//	
//}