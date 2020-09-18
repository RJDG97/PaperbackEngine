#include "Health.h"
#include "Physics.h"
#include <iostream>

Health::Health() {
	current_health_ = maximum_health_ = 5;
}

void Health::init() {
	// Push back component into container
	PHYSICS->HPs.push_back(*this); // Containers that encapsulate all components should be in a general system (Possibly core)
}

// Consult sample engine as to purpose 
void Health::PublishResults() {
	
}