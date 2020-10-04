#include "Components/Health.h"
#include "Systems/Physics.h"
#include <iostream>

Health::Health() {
	current_health_ = maximum_health_ = 5;
}

void Health::Init() {

}

void Health::Serialize(std::stringstream& data) {

	data >> maximum_health_;

	current_health_ = maximum_health_;
}