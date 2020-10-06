#include "Components/Scale.h"

Scale::Scale() : scale_{}
{}

void Scale::Init() {
	// Create the map afterwards
	//GRAPHICS->Scales[Component::GetOwner()->GetID()] = *this;
}

Vector2D Scale::GetScale() const {
	return scale_;
}

void Scale::SetScale(const Vector2D& new_scale) {
	scale_ = new_scale;
}

void Scale::Serialize(std::stringstream& data) {

	data >> scale_.x >> scale_.y;
}