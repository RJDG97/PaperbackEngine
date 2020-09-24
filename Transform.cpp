#include "Transform.h"
#include "Vector2D.h"
#include "Physics.h"

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

//void Transform::Serialize(ISerializer& str) {
//	
//}