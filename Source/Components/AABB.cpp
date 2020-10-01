#include "Components/AABB.h"
#include "Systems/Collision.h"
#include "Systems/Debug.h"
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
	//if (Component::GetOwner()
	COLLISION->AddAABBComponent(Component::GetOwner()->GetID(), this);
}

void AABB::PublishResults() {
	//transform->_position = position;
}

void AABB::Serialize(std::stringstream& data) {
	// Not required since it's going to be computed
	std::cout << "Serializing AABB Component" << std::endl;
	(void)data;
	
}

AABB* AABB::Clone() {

	AABB* cloned = new AABB;

	cloned->bottom_left_ = bottom_left_;
	cloned->top_right_ = top_right_;

	return cloned;
}