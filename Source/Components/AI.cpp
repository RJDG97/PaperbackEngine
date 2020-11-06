#include "Components/AI.h"
#include "Manager/ComponentManager.h"
#include "Script/GeneralScripts.h"
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

	writer->Key("range");
	writer->String(std::to_string(range_).c_str());

	writer->Key("speed");
	writer->String(std::to_string(speed_).c_str());

	writer->EndObject();
}

void AI::DeSerialize(std::stringstream& data) {

	std::string type;

	data >> type >> range_ >> speed_;
	type_ = GeneralScripts::GetType(type);
	
}

void AI::DeSerializeClone(std::stringstream& data) {

	std::string type;

	// clone data will be for number of destinations and destinations
	data >> type >> range_ >> speed_ >> num_destinations_;

	type_ = GeneralScripts::GetType(type);

	DEBUG_ASSERT((num_destinations_ >= 2), "Empty destinations in JSON");

	destinations_.resize(num_destinations_);

	for (size_t i = 0; i < num_destinations_; ++i) {
		data >> destinations_[i].x >> destinations_[i].y;
	}

	current_destination_ = destinations_.begin();

	DEBUG_ASSERT((current_destination_ != destinations_.end()), "Empty destinations in JSON");
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

AI::AIState AI::GetState()
{
	return state_;
}

void AI::SetState(AIState state)
{
	state_ = state;
}

float AI::GetRange()
{
	return range_;
}

void AI::SetRange(float range)
{
	range_ = range;
}

int AI::GetAtk()
{
	return attackpower_;
}

void AI::SetAtk(int atk)
{
	attackpower_ = atk;
}

float AI::GetSpeed()
{
	return speed_;
}

void AI::SetSpeed(float speed)
{
	speed_ = speed;
}

size_t AI::GetNumDes()
{
	return num_destinations_;
}

void AI::SetNumDes(size_t numdes)
{
	num_destinations_ = numdes;
}

std::vector<Vector2D>& AI::GetDestinations()
{
	return destinations_;
}

void AI::SetDestinations(std::vector<Vector2D> des)
{
	num_destinations_ = des.size();
	std::copy(std::begin(des), std::end(des), std::back_inserter(destinations_));
}

DestinationIt AI::GetCurrentDes()
{
	return current_destination_;
}

void AI::SetCurrentDes(DestinationIt Cdes)
{
	//if(Cdes == destinations_.end())
	//	current_destination_ = destinations_.begin();
	current_destination_ = Cdes;
}

AI::AIType AI::GetType()
{
	return type_;
}

Vector2D AI::GetPlayerLastPos()
{
	return player_last_pos_;
}

void AI::SetPlayerLastPos(Vector2D pos)
{
	player_last_pos_ = pos;
}