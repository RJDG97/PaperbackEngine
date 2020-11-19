#include "Components/Health.h"

#include "Systems/Physics.h"
#include "Systems/Debug.h"

#include "Manager/ComponentManager.h"

#include "Engine/Core.h"

#include <iostream>

Health::Health() {
	current_health_ = maximum_health_ = 5;
}

Health::~Health() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Health>(Component::GetOwner()->GetID());
}

void Health::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Health>(Component::GetOwner()->GetID(), this);
}

void Health::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Health");

	writer->Key("maximum health");
	writer->String(std::to_string(maximum_health_).c_str());

	writer->EndObject();
}

void Health::DeSerialize(std::stringstream& data) {
	// Serialize health
	data >> maximum_health_;
	current_health_ = maximum_health_;
}

void Health::DeSerializeClone(std::stringstream& data) {
	DeSerialize(data);
}

std::shared_ptr<Component> Health::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Health Component\n");

	std::shared_ptr<Health> cloned = std::make_shared<Health>();

	cloned->current_health_ = current_health_;
	cloned->maximum_health_ = maximum_health_;

	return cloned;
}

void Health::SetCurrentHealth(int newHealth)
{
	current_health_ = newHealth;
}

int Health::GetCurrentHealth()
{
	return current_health_;
}

int Health::GetMaxHealth()
{
	return maximum_health_;
}

void Health::SetMaxHealth(int newHealth){

	maximum_health_ = newHealth;
}
