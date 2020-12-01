#include "Engine/Core.h"
#include "Components/LogicComponent.h"
#include "Manager/ComponentManager.h"

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

	//writer->Key("entity");
	//writer->String(entity_.c_str());

	writer->Key("number_of_fns");
	writer->String(std::to_string(size_).c_str());

	for (auto& [name, function] : my_logic_) {
		
		writer->Key("name");
		writer->String(name.c_str());

		writer->Key("function");
		writer->String(function.c_str());
	}

	writer->EndObject();
}


void LogicComponent::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	Serialize(writer);
}


void LogicComponent::DeSerialize(std::stringstream& data) {

	//data >> entity_ >> size_;
	data >> size_;

	std::string name, fn_name;
	//LogicUpdate logic;

	for (size_t i = 0; i < size_; ++i) {

		data >> name >> fn_name;
		//SetLogicFn(logic, fn_name, entity_);

		my_logic_.emplace(name, fn_name);
	}
}


void LogicComponent::DeSerializeClone(std::stringstream& data) {
	
	// should just clone from archetype
	(void)data;
}


const std::string& LogicComponent::GetLogic(const std::string& fn) {

	if (my_logic_.find(fn) != my_logic_.end()) {

		return my_logic_[fn];
	}

	return {};
}


std::shared_ptr<Component> LogicComponent::Clone() {

	std::shared_ptr<LogicComponent> clone = std::make_shared<LogicComponent>();

	clone->size_ = size_;
	clone->my_logic_ = my_logic_;

	return clone;
}