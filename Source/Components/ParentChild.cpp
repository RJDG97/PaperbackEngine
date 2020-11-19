#include "Engine/Core.h"
#include "Systems/Factory.h"
#include "Systems/Debug.h"
#include "Components/ParentChild.h"

void ParentChild::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<ParentChild>(Component::GetOwner()->GetID(), this);
}

ParentChild::~ParentChild() {
	
	CORE->GetManager<ComponentManager>()->RemoveComponent<ParentChild>(Component::GetOwner()->GetID());
}


void ParentChild::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("ParentChild");

	writer->Key("elements");
	writer->String( std::to_string(to_clone_.size()).c_str() );

	for (size_t i = 0; i < to_clone_.size(); ++i) {

		writer->Key("to_clone");
		writer->String( (to_clone_[i] + " ").c_str() );
	}

	writer->EndObject();
}


void ParentChild::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	// TBC
	Serialize(writer);
}


void ParentChild::DeSerialize(std::stringstream& data) {

	(void)data;
}

void ParentChild::DeSerializeClone(std::stringstream& data) {
	
	data >> number_of_children_;

	DEBUG_ASSERT((number_of_children_ >= 1), "There are no children..");

	to_clone_.resize(number_of_children_);

	for (size_t i = 0; i < number_of_children_; ++i) {

		data >> to_clone_[i];
	}

	// Resize the child vector
	children_.resize(number_of_children_);

	for (size_t i = 0; i < to_clone_.size(); ++i) {
		// Clone a child archetype based on input string and insert it into the
		// child vector
		children_[i] = CORE->GetSystem<EntityFactory>()->CloneArchetype(to_clone_[i]);
	}
}

std::shared_ptr<Component> ParentChild::Clone() {

	return std::make_shared<ParentChild>();
}