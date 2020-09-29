#include "Components/AABB.h"
#include "Systems/Collision.h"
#include <iostream>

AABB::AABB() : top_right_{},
			   bottom_left_{}
{}

AABB::~AABB() {

	//if (Component::GetOwner())
	COLLISION->RemoveAABBComponent(Component::GetOwner()->GetID());
}

void AABB::Init() {
	// Create the map afterwards
	//COLLISION->AABBs[Component::GetOwner()->GetID()] = *this;
	//if (Component::GetOwner())
	COLLISION->AddAABBComponent(Component::GetOwner()->GetID(), this);
}

void AABB::PublishResults() {
	//transform->_position = position;
}

void AABB::Serialize(std::stringstream& data) {
	std::cout << "Serializing AABB Component" << std::endl;

	// Not required since it's going to be computed
	/*
	data >> top_right_.x >> top_right_.y >> bottom_left_.x >> bottom_left_.y;
	std::cout << "Top Right: " << top_right_.x << top_right_.y
			  << "Bottom Left: " << bottom_left_.x << bottom_left_.y
			  << std::endl;
	*/
}

AABB* AABB::Clone() {

	AABB* cloned = new AABB;

	cloned->bottom_left_ = bottom_left_;
	cloned->top_right_ = top_right_;

	return cloned;
}