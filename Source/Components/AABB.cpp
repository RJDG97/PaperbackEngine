/**********************************************************************************
*\file         AABB.cpp
*\brief        Contains definition of functions and variables used for
*			   the AABB Component
* 
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/AABB.h"
#include "Engine/Core.h"
#include "Systems/Collision.h"
#include "Systems/Debug.h"
#include <iostream>

AABB::AABB() : 
	top_right_{},
	bottom_left_{},
	scale_{ 0.5f,0.5f },
	layer_{ 0 },
	alive_{ true }
{}

AABB::~AABB() {

	CORE->GetSystem<Collision>()->RemoveAABBComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<AABB>(Component::GetOwner()->GetID());
}

void AABB::Init() {

	CORE->GetSystem<Collision>()->AddAABBComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<AABB>(Component::GetOwner()->GetID(), this);
}

void AABB::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("AABB");

	writer->Key("scale");
	writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

	writer->Key("layer");
	writer->String((std::to_string(layer_)).c_str());

	writer->EndObject();
}

void AABB::DeSerialize(std::stringstream& data) {
	
	data >> scale_.x >> scale_.y >> layer_;
}

void AABB::DeSerializeClone(std::stringstream& data) {

	DeSerialize(data);
}

void AABB::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	Serialize(writer);
}

std::shared_ptr<Component> AABB::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning AABB Component\n");
	
	std::shared_ptr<AABB> cloned = std::make_shared<AABB>();

	cloned->bottom_left_ = bottom_left_;
	cloned->top_right_ = top_right_;
	cloned->scale_ = scale_;
	cloned->layer_ = layer_;

	return cloned;
}

Vector2D AABB::GetAABBScale(){

	return scale_;
}

void AABB::SetAABBScale(Vector2D newscale){

	scale_ = newscale;
}

size_t AABB::GetLayer() const {

	return layer_;
}

void AABB::SetAlive(bool status) {

	alive_ = status;
}

bool AABB::GetAlive() const {

	return alive_;
}

bool AABB::IsColliding() const {

	return collided;
}
