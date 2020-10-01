#include "Components/Health.h"
#include "Systems/Physics.h"
#include <iostream>

Health::Health() {
	current_health_ = maximum_health_ = 5;
}

void Health::Init() {
	// Push back component into container
	//PHYSICS->HPs.push_back(*this); // Containers that encapsulate all components should be in a general system (Possibly core)
	//PHYSICS->HPs_[Component::GetOwner()->GetID()] = *this;
}

// Consult sample engine as to purpose 
void Health::PublishResults() {
	
}

//void Health::Serialize(ISerializer& str) {
	
//}