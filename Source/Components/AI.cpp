#include <iostream>
#include "Components/AI.h"
#include "Manager/AIManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/LogicSystem.h"
#include "Engine/Core.h"

AI::AI() : 
	type_{}, state_{AI::AIState::Patrol}
{}

AI::~AI() {

	//CORE->GetSystem<LogicSystem>()->RemoveAIComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<AI>(Component::GetOwner()->GetID());
}

void AI::Init() {
	//CORE->GetSystem<LogicSystem>()->AddAIComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<AI>(Component::GetOwner()->GetID(), this);
}

void AI::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("AI");

	writer->EndObject();
}

void AI::DeSerialize(std::stringstream& data) {

	std::string type;

	data >> type >> range_ >> attackpower_;

	type_ = CORE->GetManager<AIManager>()->GetType(type);
}

void AI::DeSerializeClone(std::stringstream& data) {

	(void)data;
	//DEBUG_ASSERT((current_destination_ != destinations_.end()), "Empty destinations in JSON");
}

AI::AIState AI::GetState()
{
	return state_;
}

void AI::SetState(AIState state)
{
	state_ = state;
}

std::shared_ptr<Component> AI::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning AI Component\n");

	std::shared_ptr<AI> cloned = std::make_shared<AI>();

	cloned->type_ = type_;
	cloned->range_ = range_;
	cloned->attackpower_ = attackpower_;

	return cloned;
}