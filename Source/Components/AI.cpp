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

	writer->Key("speed");
	writer->String(std::to_string(speed_).c_str());

	writer->EndObject();
}

void AI::DeSerialize(std::stringstream& data) {

	std::string type;

	data >> type >> speed_;

	type_ = CORE->GetManager<AIManager>()->GetType(type);
	
}

void AI::DeSerializeClone(std::stringstream& data) {

	std::string type;

	// clone data will be for number of destinations and destinations
	data >> type >> speed_ >> num_destinations_;

	type_ = CORE->GetManager<AIManager>()->GetType(type);

	DEBUG_ASSERT((num_destinations_ >= 2), "Empty destinations in JSON");

	destinations_.resize(num_destinations_);

	for (size_t i = 0; i < num_destinations_; ++i) {
		data >> destinations_[i].x >> destinations_[i].y;
	}

	current_destination_ = destinations_.begin();

	DEBUG_ASSERT((current_destination_ != destinations_.end()), "Empty destinations in JSON");
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

	cloned->speed_ = speed_;
	cloned->num_destinations_ = num_destinations_;
	cloned->destinations_.reserve(destinations_.size());
	std::copy(std::begin(destinations_), std::end(destinations_), std::back_inserter(cloned->destinations_));
	cloned->current_destination_ = cloned->destinations_.begin();

	return cloned;
}