#include <iostream>
#include "Manager/ComponentManager.h"
#include "Components/Motion.h"
#include "Systems/Physics.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"

Motion::Motion() : 
	velocity_{},
	acceleration_{},
	mass_{},
	is_left_{ false },
	alive_{ true }
{}

Motion::~Motion() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Motion>(Component::GetOwner()->GetID());
}

void Motion::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Motion>(Component::GetOwner()->GetID(), this);
}

void Motion::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Motion");

	//used for serializing for cloning
	writer->Key("mass");
	writer->String(std::to_string(mass_).c_str());

	writer->Key("alive");
	writer->String(std::to_string(alive_).c_str());

	writer->EndObject();
}

void Motion::DeSerialize(std::stringstream& data) {
	std::cout << "Entered DeSerialize Motion w/ stream" << std::endl;
	
	data >> mass_ >> alive_;

	inv_mass_ = mass_ == 0.0f ? 0.0f : 1 / mass_;
}


std::shared_ptr<Component> Motion::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Motion Component\n");

	std::shared_ptr<Motion> cloned = std::make_shared<Motion>();

	cloned->acceleration_ = acceleration_;
	cloned->velocity_ = velocity_;
	cloned->mass_ = mass_;
	cloned->inv_mass_ = inv_mass_;
	cloned->alive_ = alive_;

	return cloned;
}

float Motion::GetMass()
{
	return mass_;
}

void Motion::SetMass(float newMass)
{
	mass_ = newMass;
}

bool Motion::GetIsLeft() {

	return is_left_;
}

void Motion::SetIsLeft(bool status) {

	is_left_ = status;
}

Vector2D Motion::GetVelocity() {
	return velocity_;
}