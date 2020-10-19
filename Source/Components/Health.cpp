#include "Components/Health.h"
#include "Systems/Physics.h"
#include "Systems/Debug.h"
#include <iostream>

Health::Health() {
	current_health_ = maximum_health_ = 5;
}

void Health::Init() {

}

void Health::DeSerialize(std::stringstream& data) {
	// Serialize health
	data >> maximum_health_;
	current_health_ = maximum_health_;
}

void Health::DeSerializeClone(std::stringstream& data) {
	DeSerialize(data);
}

std::shared_ptr<Component> Health::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Health Component\n");

	std::shared_ptr<Health> cloned = std::make_shared<Health>();

	cloned->current_health_ = current_health_;
	cloned->maximum_health_ = maximum_health_;

	return cloned;
}