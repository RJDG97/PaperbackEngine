#include "Components/Scale.h"

Scale::Scale() : scale_{}
{}

void Scale::Init() {
	// Create the map afterwards
	//GRAPHICS->Scales[Component::GetOwner()->GetID()] = *this;
}

void Scale::PublishResults() {
	//transform->_position = position;
}

Vector2D Scale::GetScale() const {
	return scale_;
}

void Scale::Serialize(std::stringstream& data) {

	data >> scale_.x >> scale_.y;
}