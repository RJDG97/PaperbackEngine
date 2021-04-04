/**********************************************************************************
*\file         AI.cpp
*\brief        Contains definition of functions and variables used for
*			   the AI Component
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/AI.h"
#include "Manager/ComponentManager.h"
#include "Script/ScriptList.h"
#include "Systems/LogicSystem.h"
#include "Engine/Core.h"

AI::AI() : root_(nullptr){}

AI::~AI() {
	if (root_)
		delete root_;
	CORE->GetManager<ComponentManager>()->RemoveComponent<AI>(Component::GetOwner()->GetID());
}

void AI::Init() {
	CORE->GetManager<ComponentManager>()->AddComponent<AI>(Component::GetOwner()->GetID(), this);
}

void AI::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("AI");

	writer->Key("level");
	writer->String(std::to_string(level_).c_str());

	writer->Key("range");
	writer->String(std::to_string(range_).c_str());

	writer->Key("speed");
	writer->String(std::to_string(speed_).c_str());

	writer->EndObject();
}

void AI::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("AI");

	writer->Key("level");
	writer->String(std::to_string(level_).c_str());

	writer->Key("range");
	writer->String(std::to_string(range_).c_str());

	writer->Key("speed");
	writer->String(std::to_string(speed_).c_str());

	writer->Key("dest");
	writer->String(std::to_string(num_destinations_).c_str());

	for (size_t i = 0; i < num_destinations_; ++i) {
		
		writer->Key(("destination" + std::to_string(i + 1)).c_str());
		writer->String((std::to_string(destinations_[i].x) + " " + std::to_string(destinations_[i].y)).c_str());
	}

	writer->EndObject();
}

void AI::DeSerialize(std::stringstream& data) {

	std::string type;
	data >> type >> level_ >> range_ >> speed_;
}

void AI::SetRoot(AIType type){
	switch (type) 
	{
	case StagBeetle:
		root_ = new Stag_Tree::StagRoot(GetOwner()->GetID());
		break;
	case Mite:
		root_ = new Mite_Tree::MiteRoot(GetOwner()->GetID());
		break;
	case Cutout:
		root_ = new Cutout::CutoutRoot(GetOwner()->GetID());
		break;
	}
}

AI::AIType AI::GetType(std::string type)
{
	if (type == "Stag_Beetle")
		return StagBeetle;
	else if (type == "Mite")
		return Mite;
	else if (type == "Cutout")
		return Cutout;
	// replace with exception
	return StagBeetle;
}

void AI::DeSerializeClone(std::stringstream& data) {

	std::string type;

	// clone data will be for number of destinations and destinations
	data >> type >> level_ >> range_ >> speed_ >> num_destinations_;
	type_ = GetType(type);
	SetRoot(type_);
	//DEBUG_ASSERT((num_destinations_ >= 2), "Empty destinations in JSON");

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

	cloned->alive_ = true;
	cloned->type_ = type_;
	cloned->level_ = level_;
	cloned->range_ = range_;
	cloned->attackpower_ = attackpower_;
	cloned->state_ = state_;
	cloned->speed_ = speed_;
	cloned->num_destinations_ = num_destinations_;
	cloned->destinations_.reserve(destinations_.size());
	std::copy(std::begin(destinations_), std::end(destinations_), std::back_inserter(cloned->destinations_));
	std::copy(std::begin(path_), std::end(path_), std::back_inserter(cloned->path_));
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
	current_destination_ = destinations_.begin();
}

DestinationIt AI::GetCurrentDes()
{
	return current_destination_;
}

void AI::SetCurrentDes(DestinationIt Cdes)
{
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

Vector2D AI::GetNewDestination()
{
	return new_node_;
}

void AI::SetNewDestination(Vector2D newnode)
{
	new_node_ = newnode;
}

std::vector<Vector2D>& AI::GetPath()
{
	return path_;
}

void AI::SetPath(std::vector<Vector2D>& path)
{
	path_ = path;
}

Time_Channel& AI::GetTimer()
{
	return recovery_timer_;
}

bool AI::GetLife()
{
	return alive_;
}

void AI::SetLife(bool life)
{
	alive_ = life;
}