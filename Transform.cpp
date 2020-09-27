#include "Transform.h"
#include "Vector2D.h"
#include "Physics.h"
#include <sstream>
#include <iostream>

Transform::Transform() : 
	position_{ },
	rotation_{ } 
{}

Transform::~Transform() {

	PHYSICS->RemoveTransformComponent(Component::GetOwner()->GetID());
}

void Transform::Init() {
	//PHYSICS->Transforms.push_back(*this);
	//PHYSICS->Transforms_[Component::GetOwner()->GetID()] = *this;
	PHYSICS->AddTransformComponent(Component::GetOwner()->GetID(), this);
}

void Transform::PublishResults() {
	//transform->_position = position;
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