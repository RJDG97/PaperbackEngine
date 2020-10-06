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

void BasicAI::Serialize(std::stringstream& data) {
	std::cout << "Entered Serialize BasicAI w/ stream" << std::endl;

	// 1st data will be magnitude of velocity
	// 2nd data will be for number of destinations
	data >> speed >> num_destinations_;
	
	DEBUG_ASSERT((num_destinations_ >= 2), "Empty destinations in JSON");

	destinations_.resize(num_destinations_);

	for (size_t i = 0; i < num_destinations_; ++i) {
		data >> destinations_[i].x >> destinations_[i].y;
	}

	current_destination_ = destinations_.begin();

	DEBUG_ASSERT((current_destination_ != destinations_.end()), "Empty destinations in JSON");
}