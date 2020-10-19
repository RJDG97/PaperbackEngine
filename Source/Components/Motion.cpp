#include <iostream>
#include "Components/Motion.h"
#include "Systems/Physics.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"

Motion::Motion() : velocity_{},
				   acceleration_{}
{}

Motion::~Motion() {

	CORE->GetSystem<Physics>()->RemoveMotionComponent(Component::GetOwner()->GetID());
	CORE->GetSystem<Collision>()->RemoveMotionComponent(Component::GetOwner()->GetID());

}

void Motion::Init() {
	// Create the map afterwards
	//PHYSICS->Motions[Component::GetOwner()->GetID()] = *this;
	CORE->GetSystem<Physics>()->AddMotionComponent(Component::GetOwner()->GetID(), this);
	CORE->GetSystem<Collision>()->AddMotionComponent(Component::GetOwner()->GetID(), this);
}

void Motion::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Motion");

	//used for serializing for cloning
	/*writer->Key("velocity");
	writer->String((std::to_string(velocity_.x) + " " + std::to_string(velocity_.y)).c_str());

	writer->Key("acceleration");
	writer->String((std::to_string(acceleration_.x) + " " + std::to_string(acceleration_.y)).c_str());
	*/

	writer->EndObject();
}

void Motion::DeSerialize(std::stringstream& data) {
	std::cout << "Entered Serialize Motion w/ stream" << std::endl;
	
	data >> velocity_.x >> velocity_.y >> acceleration_.x >> acceleration_.y;
}


std::shared_ptr<Component> Motion::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Motion Component\n");

	std::shared_ptr<Motion> cloned = std::make_shared<Motion>();

	cloned->acceleration_ = acceleration_;
	cloned->velocity_ = velocity_;

	return cloned;
}