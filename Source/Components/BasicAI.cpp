#include <iostream>
#include "Components/BasicAI.h"
#include "Systems/Game.h"
#include "Engine/Core.h"

BasicAI::BasicAI() : 
	num_destinations_{},
	destinations_{},
	current_destination_{}
{}

BasicAI::~BasicAI() {

	CORE->GetSystem<Game>()->RemoveBasicAIComponent(Component::GetOwner()->GetID());

}

void BasicAI::Init() {
	// Create the map afterwards
	//PHYSICS->Motions[Component::GetOwner()->GetID()] = *this;
	CORE->GetSystem<Game>()->AddBasicAIComponent(Component::GetOwner()->GetID(), this);
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