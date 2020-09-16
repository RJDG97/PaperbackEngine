#include "Health.h"
#include "Physics.h"
#include <iostream>

Health::Health() {
	currentHealth = maximumHealth = 5;
}

void Health::init() {

	PHYSICS->HPs.push_back(*this);
}

void Health::PublishResults() {
	
}