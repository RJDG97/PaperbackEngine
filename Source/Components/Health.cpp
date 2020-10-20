#include "Components/Health.h"
#include "Systems/Physics.h"
#include "Systems/Debug.h"
#include <iostream>

Health::Health() {
	current_health_ = maximum_health_ = 5;
}

void Health::Init() {

}

void Health::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Health");

	writer->Key("maximum health");
	writer->String(std::to_string(maximum_health_).c_str());

	writer->EndObject();
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