#include "AABB.h"
#include "Collision.h"

AABB::AABB() : top_right_{},
			   bottom_left_{}
{}

void AABB::Init() {
	// Create the map afterwards
	//COLLISION->AABBs[Component::GetOwner()->GetID()] = *this;
	COLLISION->AddAABBComponent(Component::GetOwner()->GetID(), this);
}

void AABB::PublishResults() {
	//transform->_position = position;
}

//void AABB::Serialize(ISerializer& str) {
//	
//}