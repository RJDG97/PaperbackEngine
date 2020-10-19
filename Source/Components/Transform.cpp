#include "Components/Transform.h"
#include "MathLib/Vector2D.h"
#include "Systems/Physics.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"
#include <iostream> 
// originally sstream

Transform::Transform() : 
	position_{ },
	rotation_{ } 
{}

Transform::~Transform() {

	CORE->GetSystem<Physics>()->RemoveTransformComponent(Component::GetOwner()->GetID());
	CORE->GetSystem<Collision>()->RemoveTransformComponent(Component::GetOwner()->GetID());
}

void Transform::Init() {

	CORE->GetSystem<Physics>()->AddTransformComponent(Component::GetOwner()->GetID(), this);
	CORE->GetSystem<Collision>()->AddTransformComponent(Component::GetOwner()->GetID(), this);
}

void Transform::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Transform");

	writer->Key("position");
	writer->String((std::to_string(position_.x)+ " " + std::to_string(position_.y)).c_str());

	writer->Key("rotation");
	writer->String(std::to_string(rotation_).c_str());

	writer->EndObject();
}

void Transform::DeSerialize(std::stringstream& data) {

	std::cout << "Entered Serialize Transform w/ stream" << std::endl;
	
	data >> position_.x >> position_.y >> rotation_;

	std::cout << "Position read: " << position_.x << ", " << position_.y << std::endl;
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

std::shared_ptr<Component> Transform::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Transform Component\n");
	std::shared_ptr<Transform> cloned = std::make_shared<Transform>();

	cloned->position_ = position_;
	cloned->rotation_ = rotation_;

	return cloned;
}