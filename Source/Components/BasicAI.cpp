#include <iostream>
#include "Components/BasicAI.h"
#include "Manager/ComponentManager.h"
#include "Systems/Game.h"
#include "Engine/Core.h"

BasicAI::BasicAI() : 
	num_destinations_{},
	destinations_{},
	current_destination_{}
{}

BasicAI::~BasicAI() {

	//CORE->GetSystem<Game>()->RemoveBasicAIComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<BasicAI>(Component::GetOwner()->GetID());
}

void BasicAI::Init() {
	// Create the map afterwards
	//CORE->GetSystem<Game>()->AddBasicAIComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<BasicAI>(Component::GetOwner()->GetID(), this);
}

void BasicAI::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("BasicAI");

	writer->Key("speed");
	writer->String(std::to_string(speed).c_str());

	writer->EndObject();
}

void BasicAI::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("BasicAI");


	writer->Key("size");
	writer->String(std::to_string(num_destinations_).c_str());

	for (DestinationIt it = destinations_.begin(); it != destinations_.end(); ++it) {
		
		writer->Key("destination");
		writer->String((std::to_string(it->x) + " " + std::to_string(it->y)).c_str());
	}

	writer->EndObject();
}

void BasicAI::DeSerialize(std::stringstream& data) {
	
	// assume archetype only sets speed
	// 1st data will be magnitude of velocity
	data >> speed;
}

void BasicAI::DeSerializeClone(std::stringstream& data) {

	// clone data will be for number of destinations and destinations
	data >> num_destinations_;

	DEBUG_ASSERT((num_destinations_ >= 2), "Empty destinations in JSON");

	destinations_.resize(num_destinations_);

	for (size_t i = 0; i < num_destinations_; ++i) {
		data >> destinations_[i].x >> destinations_[i].y;
	}

	current_destination_ = destinations_.begin();

	DEBUG_ASSERT((current_destination_ != destinations_.end()), "Empty destinations in JSON");
}

std::shared_ptr<Component> BasicAI::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning BasicAI Component\n");

	std::shared_ptr<BasicAI> cloned = std::make_shared<BasicAI>();

	cloned->speed = speed;
	cloned->num_destinations_ = num_destinations_;
	cloned->destinations_.reserve(destinations_.size());
	std::copy(std::begin(destinations_), std::end(destinations_), std::back_inserter(cloned->destinations_));
	cloned->current_destination_ = cloned->destinations_.begin();

	return cloned;
}

std::vector<Vector2D>& BasicAI::GetDestinations()
{
	return destinations_;
}

void BasicAI::SetDestinations(std::vector<Vector2D> des)
{
	num_destinations_ = des.size();
	std::copy(std::begin(des), std::end(des), std::back_inserter(destinations_));
}

DestinationIt BasicAI::GetCurrentDes()
{
	return current_destination_;
}

void BasicAI::SetCurrentDes(DestinationIt Cdes)
{
	current_destination_ = Cdes;
}

