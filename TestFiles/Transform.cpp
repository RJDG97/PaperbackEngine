#include "Transform.h"
#include "../Vector2D.h"
#include "Physics.h"

Transform::Transform() {
	_position = Vec2{ 0,0 };
	_rotation = 0.0f;
}

void Transform::init() {
	PHYSICS->Transforms.push_back(*this);
}

void Transform::PublishResults() {
	//transform->_position = position;
}

//void Position::serialize(ISerializer& str) {
//	
//}