#include "Components/Transform.h"
#include "MathLib/Vector2D.h"
#include "Systems/Physics.h"
#include "Engine/Core.h"
#include <iostream> 
// originally sstream

Transform::Transform() : 
	position_{ },
	rotation_{ } 
{}

Transform::~Transform() {

	CORE->GetSystem<Physics>()->RemoveTransformComponent(Component::GetOwner()->GetID());
}

void Transform::Init() {
	//PHYSICS->Transforms.push_back(*this);
	//PHYSICS->Transforms_[Component::GetOwner()->GetID()] = *this;
	CORE->GetSystem<Physics>()->AddTransformComponent(Component::GetOwner()->GetID(), this);
}

void Transform::Serialize(std::stringstream& data) {
	/*
	rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin()
	it2->name.GetString() << ": " << it2->value.GetString()
	*/
	std::cout << "Entered Serialize Transform w/ stream" << std::endl;
	
	data >> position_.x >> position_.y >> rotation_;

	std::cout << "Position read: " << position_.x << ", " << position_.y << std::endl;
}

void Transform::SerializeClone(std::stringstream& data) {
	Serialize(data);
}

float Transform::GetRotation() const {

	return rotation_;
}

void Transform::SetRotation(const float& rot) {

	rotation_ = rot;
}

std::shared_ptr<Component> Transform::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Transform Component\n");
	std::shared_ptr<Transform> cloned = std::make_shared<Transform>();

	cloned->position_ = position_;
	cloned->rotation_ = rotation_;

	return cloned;
}