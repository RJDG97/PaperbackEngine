#include <iostream>
#include "Components/AI.h"
#include "Systems/LogicSystem.h"
#include "Engine/Core.h"

AI::AI() : aitype{}, state{}
{}

AI::~AI() {

	CORE->GetSystem<LogicSystem>()->RemoveAIComponent(Component::GetOwner()->GetID());

}

void AI::Init() {
	CORE->GetSystem<LogicSystem>()->AddAIComponent(Component::GetOwner()->GetID(), this);
}

void AI::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("AI");

	writer->EndObject();
}

void AI::DeSerialize(std::stringstream& data) {
}

void AI::DeSerializeClone(std::stringstream& data) {

	//DEBUG_ASSERT((current_destination_ != destinations_.end()), "Empty destinations in JSON");
}

std::shared_ptr<Component> AI::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning BasicAI Component\n");

	std::shared_ptr<AI> cloned = std::make_shared<AI>();

	return cloned;
}