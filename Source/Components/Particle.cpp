#include "Components/Particle.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"


Particle::Particle() :
	lifetime_{},
	alive_{ false },
	spawner_{}
{  }


Particle::~Particle() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Particle>(Component::GetOwner()->GetID());
}


void Particle::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Particle>(Component::GetOwner()->GetID(), this);
}


void Particle::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Particle");

	writer->EndObject();
}


void Particle::DeSerialize(std::stringstream& data) {
	
	
}


std::shared_ptr<Component> Particle::Clone() {

	return std::make_shared<Particle>();
}


bool Particle::IsAlive() const {

	return alive_;
}