#include "Engine/Core.h"
#include "Script/Child_Script.h"
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

	writer->Key("function");
	writer->String(function_.c_str());

	writer->EndObject();
}


void LogicComponent::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	Serialize(writer);
}


void LogicComponent::DeSerialize(std::stringstream& data) {

	data >> function_;

	SetLogicFn(MyUpdate, function_);
}


void LogicComponent::DeSerializeClone(std::stringstream& data) {
	
	// should just clone from archetype
}


std::shared_ptr<Component> LogicComponent::Clone() {

	std::shared_ptr<LogicComponent> clone = std::make_shared<LogicComponent>();

	clone->function_ = function_;
	clone->MyUpdate = MyUpdate;

	return clone;
}