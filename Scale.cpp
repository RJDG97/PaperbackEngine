#include "Scale.h"

Scale::Scale() : width_scale_{},
			     height_scale_{}
{}

void Scale::Init() {
	// Create the map afterwards
	//GRAPHICS->Scales[Component::GetOwner()->GetID()] = *this;
}

void Scale::PublishResults() {
	//transform->_position = position;
}

//void Scale::Serialize(ISerializer& str) {
//	
//}