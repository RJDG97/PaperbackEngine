/**********************************************************************************
*\file         ParentChild.cpp
*\brief        Contains definition of functions and variables used for
*			   the ParentChild Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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

	writer->Key("my_name");
	writer->String(name_.c_str());

	writer->EndObject();
}


void ParentChild::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	Serialize(writer); // Might not need
}


void ParentChild::DeSerialize(std::stringstream& data) {

	data >> name_;
}

void ParentChild::DeSerializeClone(std::stringstream& data) {
	
	DeSerialize(data);
}

std::shared_ptr<Component> ParentChild::Clone() {

	std::shared_ptr<ParentChild> cloned = std::make_shared<ParentChild>();
	cloned->name_ = name_;

	return cloned;
}

std::list<Entity*>& ParentChild::GetChildren() {
	
	return children_;
}

std::string ParentChild::GetName() const {
	
	return name_;
}

void ParentChild::AddChild(const EntityID& id) {
	
	EntityManager* entity_mgr = &*CORE->GetManager<EntityManager>();
	Entity* child = entity_mgr->GetEntity(id);

	for (auto& a_child : children_) {
		if (a_child->GetID() == id)
			return;
	}

	if (child)
		children_.push_front(child);
}

void ParentChild::RemoveChild(const EntityID& id) {

	EntityManager* entity_mgr = &*CORE->GetManager<EntityManager>();
	Entity* child = entity_mgr->GetEntity(id);

	for (auto begin = children_.begin(); begin != children_.end(); ++begin) {
	
		if (*begin == child) {
			
			children_.remove(child);
			break;
		}
	}
}