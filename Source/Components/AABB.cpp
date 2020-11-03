#include "Components/AABB.h"
#include "Engine/Core.h"
#include "Systems/Collision.h"
#include "Systems/Debug.h"
#include <iostream>

AABB::AABB() : top_right_{},
			   bottom_left_{},
			scale_{30.0f,30.0f}
{}

AABB::~AABB() {

	CORE->GetSystem<Collision>()->RemoveAABBComponent(Component::GetOwner()->GetID());
}

void AABB::Init() {

	CORE->GetSystem<Collision>()->AddAABBComponent(Component::GetOwner()->GetID(), this);
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
	// Not required since it's going to be computed
	std::cout << "Serializing AABB Component" << std::endl;
	
	data >> scale_.x >> scale_.y >> layer_;
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
