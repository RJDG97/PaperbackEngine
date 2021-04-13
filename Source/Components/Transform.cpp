/**********************************************************************************
*\file         Transform.cpp
*\brief        Contains definition of functions and variables used for
*			   the Transform Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Transform.h"
#include "MathLib/Vector2D.h"
#include "Manager/ComponentManager.h"
#include "Systems/Physics.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"
#include <iostream> 
// originally sstream

Transform::Transform() :
	position_{ },
	rotation_{ },
	rotation_speed_{ },
	rotation_range_{ },
	offset_{ },
	aabb_offset_{}
{}

Transform::~Transform() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Transform>(Component::GetOwner()->GetID());

}

void Transform::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Transform>(Component::GetOwner()->GetID(), this);
}

void Transform::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Transform");

	writer->Key("position");
	writer->String((std::to_string(position_.x)+ " " + std::to_string(position_.y)).c_str());

	writer->Key("rotation");
	writer->String(std::to_string(rotation_).c_str());

	writer->Key("offset");
	writer->String((std::to_string(offset_.x) + " " + std::to_string(offset_.y)).c_str());

	writer->Key("aabb offset");
	writer->String((std::to_string(aabb_offset_.x) + " " + std::to_string(aabb_offset_.y)).c_str());

	writer->EndObject();
}

void Transform::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	Serialize(writer);
}

void Transform::DeSerialize(std::stringstream& data) {
	
	data >> position_.x >> position_.y >> rotation_ >> offset_.x >> offset_.y >> aabb_offset_.x >> aabb_offset_.y;
}

void Transform::DeSerializeClone(std::stringstream& data) {
	DeSerialize(data);
}

float Transform::GetRotation() const {

	return rotation_;
}

void Transform::SetRotation(const float& rot) {

	rotation_ = rot;
}

Vector2D Transform::GetPosition() const {

	return position_;
}

Vector2D* Transform::GetPositionPtr() {

	return &position_;
}

void Transform::SetPosition(const Vector2D& pos) {

	position_ = pos;
}

Vector2D Transform::GetOffset() const {

	return offset_;
}

void Transform::SetOffset(const Vector2D& offset) {

	offset_ = offset;
}

void Transform::AddOffset(const Vector2D& offset) {
	
	offset_ += offset;
}

void Transform::SetAABBOffset(const Vector2D& new_offset) {
	
	aabb_offset_ = new_offset;
}

Vector2D Transform::GetAABBOffset() const {
	
	return aabb_offset_;
}

Vector2D Transform::GetOffsetAABBPos() const {

	return position_ + aabb_offset_;
}


float Transform::GetRotationSpeed() const {
	
	return rotation_speed_;
}


void Transform::SetRotationSpeed(const float& new_speed) {
	
	rotation_speed_ = new_speed;
}


Vector2D Transform::GetRotationRange() const {
	
	return rotation_range_;
}


void Transform::SetRotationRange(const Vector2D& new_range) {

	rotation_range_ = new_range;
}


std::shared_ptr<Component> Transform::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Transform Component\n");
	std::shared_ptr<Transform> cloned = std::make_shared<Transform>();

	cloned->position_ = position_;
	cloned->rotation_ = rotation_;
	cloned->offset_ = offset_;

	return cloned;
}