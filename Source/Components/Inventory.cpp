#include "Components/Inventory.h"
#include "MathLib/Vector2D.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include <iostream> 
// originally sstream

Inventory::Inventory() :
	has_key_{ false },
	keys_{}
{}

Inventory::~Inventory() {

	//CORE->GetSystem<Physics>()->RemoveTransformComponent(Component::GetOwner()->GetID());
	//CORE->GetSystem<Collision>()->RemoveTransformComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<Inventory>(Component::GetOwner()->GetID());

}

void Inventory::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Inventory>(Component::GetOwner()->GetID(), this);
}

void Inventory::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Inventory");

	writer->EndObject();
}

void Inventory::DeSerialize(std::stringstream& data) {
	
	UNREFERENCED_PARAMETER(data);
}

bool Inventory::HasKey() const {

	return has_key_;
}

void Inventory::SetHasKey(const bool val) {

	keys_ = (val) ? keys_ + 1 : ((keys_ > 0) ? keys_ - 1 : keys_);
	has_key_ = (keys_ > 0) ? true : false;
}

size_t Inventory::GetKeys() const {

	return keys_;
}

std::shared_ptr<Component> Inventory::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Inventory Component\n");
	std::shared_ptr<Inventory> cloned = std::make_shared<Inventory>();

	cloned->has_key_ = has_key_;
	cloned->keys_ = keys_;

	return cloned;
}