#include "Components/Clickable.h"
#include "Engine/Core.h"
#include "Systems/Debug.h"
#include "Systems/Collision.h"
#include <iostream>

Clickable::Clickable() : 
	scale_{},
	top_right_{},
	bottom_left_{},
	collided_{ false },
	index_{}
{}

Clickable::~Clickable() {
	CORE->GetSystem<Collision>()->RemoveClickableComponent(Component::GetOwner()->GetID());
}

void Clickable::Init() {

	CORE->GetSystem<Collision>()->AddClickableComponent(Component::GetOwner()->GetID(), this);
}

void Clickable::DeSerialize(std::stringstream& data) {
	// Not required since it's going to be computed
	std::cout << "Serializing Clickable Component" << std::endl;
	data >> scale_.x >> scale_.y;
}

void Clickable::DeSerializeClone(std::stringstream& data) {
	
	data >> index_; // might need to include scale here
}

std::shared_ptr<Component> Clickable::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Clickable Component\n");
	
	std::shared_ptr<Clickable> cloned = std::make_shared<Clickable>();

	cloned->bottom_left_ = bottom_left_;
	cloned->top_right_ = top_right_;
	cloned->scale_ = scale_;

	return cloned;
}