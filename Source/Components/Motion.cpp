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

void Motion::DeSerialize(std::stringstream& data) {
	std::cout << "Entered Serialize Motion w/ stream" << std::endl;
	
	data >> velocity_.x >> velocity_.y >> acceleration_.x >> acceleration_.y;
}

void Motion::DeSerializeClone(std::stringstream& data) {
	DeSerialize(data);
}

std::shared_ptr<Component> Motion::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Motion Component\n");

	std::shared_ptr<Motion> cloned = std::make_shared<Motion>();

	cloned->acceleration_ = acceleration_;
	cloned->velocity_ = velocity_;

	return cloned;
}