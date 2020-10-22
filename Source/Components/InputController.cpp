#include "Components/InputController.h"
#include "Engine/Core.h"
#include "Systems/Game.h"
#include "Systems/Debug.h"
#include <iostream>

InputController::InputController() : 
	input_map_{},
	num_entries_{}
{}

InputController::~InputController() {

	CORE->GetSystem<Game>()->RemoveInputControllerComponent(Component::GetOwner()->GetID());
}

void InputController::Init() {

	CORE->GetSystem<Game>()->AddInputControllerComponent(Component::GetOwner()->GetID(), this);
}

bool InputController::VerifyKey(const std::string& name, const size_t value) {

	InputMapTypeIt it = input_map_.find(name);
	if (it != input_map_.end()) {
		
		if (it->second == value)
			return true;
	}	

	return false;
}

void InputController::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("InputController");

	//writer->Key("scale");
	//writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

	writer->EndObject();
}

void InputController::DeSerialize(std::stringstream& data) {
	
	(void)data;
}

void InputController::DeSerializeClone(std::stringstream& data) {

	//first value denotes how many times to loop
	data >> num_entries_; 
	std::string buffer;
	int key_val;

	for (size_t i = 0; i < num_entries_; ++i) {

		data >> buffer >> key_val;
		input_map_.emplace(buffer, key_val);
	}
}

std::shared_ptr<Component> InputController::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning InputController Component\n");
	
	std::shared_ptr<InputController> cloned = std::make_shared<InputController>();

	return cloned;
}