#include "Components/Attack.h"

Attack::Attack() : attack_power_{}
{}

void Attack::Init() {
	// Create the map afterwards
	//GAMELOGIC->Attacks[Component::GetOwner()->GetID()] = *this;
}

void Attack::DeSerialize(std::stringstream& data) {
	
	(void)(data);
}

void Attack::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Attack");

	writer->Key("power");
	writer->String(std::to_string(attack_power_).c_str());

	writer->EndObject();
}