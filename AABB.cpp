#include "AABB.h"

AABB::AABB() : top_left_{},
			   bottom_right_{}
{}

void AABB::init() {
	// Create the map afterwards
	//COLLISION->AABBs[Component::GetOwner()->GetID()] = *this;
}

void AABB::PublishResults() {
	//transform->_position = position;
}

//void AABB::serialize(ISerializer& str) {
//	
//}