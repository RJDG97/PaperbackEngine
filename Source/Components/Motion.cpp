#include <iostream>
#include "Manager/ComponentManager.h"
#include "Components/Motion.h"
#include "Systems/Physics.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"

Motion::Motion() : 
	velocity_{},
	acceleration_{},
	mass_{}
{}

Motion::~Motion() {

	//CORE->GetSystem<Physics>()->RemoveMotionComponent(Component::GetOwner()->GetID());
	//CORE->GetSystem<Collision>()->RemoveMotionComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<Motion>(Component::GetOwner()->GetID());
}

void Motion::Init() {

	//CORE->GetSystem<Physics>()->AddMotionComponent(Component::GetOwner()->GetID(), this);
	//CORE->GetSystem<Collision>()->AddMotionComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<Motion>(Component::GetOwner()->GetID(), this);
}

void Motion::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Motion");

	//used for serializing for cloning
	writer->Key("mass");
	writer->String(std::to_string(mass_).c_str());

	/*writer->Key("acceleration");
	writer->String((std::to_string(acceleration_.x) + " " + std::to_string(acceleration_.y)).c_str());
	*/

	writer->EndObject();
}

void Motion::DeSerialize(std::stringstream& data) {
	std::cout << "Entered DeSerialize Motion w/ stream" << std::endl;
	
	data >> mass_ >> velocity_.x >> velocity_.y >> acceleration_.x >> acceleration_.y;
	inv_mass_ = mass_ == 0.0f ? 0.0f : 1 / mass_;
}


std::shared_ptr<Component> Motion::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Motion Component\n");

	std::shared_ptr<Motion> cloned = std::make_shared<Motion>();

	cloned->acceleration_ = acceleration_;
	cloned->velocity_ = velocity_;
	cloned->mass_ = mass_;

	return cloned;
}