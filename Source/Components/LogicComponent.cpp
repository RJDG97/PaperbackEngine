#include "Engine/Core.h"
#include "Script/Player_Script.h"
#include "Components/LogicComponent.h"

void LogicComponent::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<LogicComponent>(Component::GetOwner()->GetID(), this);
}


LogicComponent::~LogicComponent() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<LogicComponent>(Component::GetOwner()->GetID());
}


void LogicComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("LogicComponent");

	writer->Key("entity");
	writer->String(entity_.c_str());

	writer->Key("number_of_fns");
	writer->String(std::to_string(size_).c_str());

	for (auto& [name, function] : my_logic_) {
		
		writer->Key("name");
		writer->String(name.c_str());

		writer->Key("function");
		writer->String(name.c_str());
	}

	writer->EndObject();
}


void LogicComponent::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	Serialize(writer);
}


void LogicComponent::DeSerialize(std::stringstream& data) {

	/*data >> function_;
	SetLogicFn(MyUpdate, function_);*/

	data >> entity_ >> size_;

	std::string name, fn_name;
	LogicUpdate logic;

	for (size_t i = 0; i < size_; ++i) {

		data >> name >> fn_name;
		SetLogicFn(logic, fn_name, entity_);

		my_logic_.emplace(name, logic);
	}
}


void LogicComponent::DeSerializeClone(std::stringstream& data) {
	
	// should just clone from archetype
}


std::shared_ptr<Component> LogicComponent::Clone() {

	std::shared_ptr<LogicComponent> clone = std::make_shared<LogicComponent>();

	clone->size_ = size_;
	clone->my_logic_ = my_logic_;

	return clone;
}