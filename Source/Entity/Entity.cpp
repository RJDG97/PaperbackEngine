#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include "Manager/EntityManager.h"
#include <algorithm>
#include "Systems/Factory.h"
#include "Engine/Core.h"
#include <iostream>
#include <functional>
#include "Systems/Debug.h"

auto ComponentSorter = [](std::shared_ptr<Component> left, std::shared_ptr<Component> right){
	
	return left->GetComponentTypeID() < right->GetComponentTypeID();
};

/******************************************************************************/
/*!
  \fn ComponentLocator()

  \brief Helper function used to seach for a component in GetComponent
*/
/******************************************************************************/
bool ComponentLocator(std::shared_ptr<Component> check, ComponentTypes id) {

	if (check->GetComponentTypeID() == id) {

		return true;
	}
	return false;
};

void Entity::Init() {

	//inits all components owned by entity and set the component's owner
	//allows each component to be initialised separate from ctor

	M_DEBUG->WriteDebugMessage("Initialising entity\n");

	for (EntityIt it = components_.begin(); it != components_.end(); ++it) {

		(*it)->owner_ = this;
		(*it)->Init();
	}

	M_DEBUG->WriteDebugMessage("Ending init\n");
};

void Entity::InitArchetype() {

	//inits all components owned by entity and set the component's owner
	//allows each component to be initialised separate from ctor

	M_DEBUG->WriteDebugMessage("Initialising archetype\n");

	for (EntityIt it = components_.begin(); it != components_.end(); ++it) {

		(*it)->owner_ = this;
	}
};

Entity::Entity() {
	// Initialise id to 0 since it will be assigned by factory
	object_id_ = 0;
	//entity_type_ = EntityTypes::NONE;
}

// Destroys all components attached to an entity
Entity::~Entity() {
	
	components_.clear();
}

ComponentArr Entity::GetComponentArr() {
	return components_;
}

// Attach a new component to the entity and sort the components according to the order in 
// ComponentTypes.h
void Entity::AddComponent(ComponentTypes type_id, std::shared_ptr<Component> component) {

	//ensure pointer is not nullptr
	DEBUG_ASSERT(component, "Component is nullptr");
	//ensure that the entity/archetype cannot have multiple of the same component
	DEBUG_ASSERT(!(HasComponent(type_id)), "Component already exist");

	component->type_id_ = type_id;
	components_.push_back(component);

	std::sort(components_.begin(), components_.end(), ComponentSorter);
}

bool Entity::HasComponent(ComponentTypes type_id) {

	for (ComponentArrIt begin = components_.begin(); begin != components_.end(); ++begin) {

		if (begin->get()->type_id_ == type_id)
			return true;
	}
	return false;
}

// Returns a pointer to a component attached to an entity
std::shared_ptr<Component> Entity::GetComponent(ComponentTypes type_id) {
	
	EntityIt result = std::find_if(components_.begin(), components_.end(), std::bind(&ComponentLocator, std::placeholders::_1, type_id));
	
	if (result != std::end(components_)) {

		return *result;
	}

	return nullptr;
}

//A more advanced type safe way of accessing components.
//Interface becomes Transform* transform = object->has(Transform);
template<typename return_type>
std::shared_ptr<return_type> Entity::GetComponentType(ComponentTypes typeId) {

	return std::dynamic_pointer_cast<return_type*>(GetComponent(typeId));
}

void Entity::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartArray();
	
	for (std::shared_ptr<Component> component : components_) {
		
		component->Serialize(writer);
	}

	//end entity
	writer->EndArray();
}

void Entity::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartArray();

	for (std::shared_ptr<Component> component : components_) {

		component->SerializeClone(writer);
	}

	//end entity
	writer->EndArray();
}

//used for creating copies from a protoype/archetype
Entity* Entity::Clone() {

	M_DEBUG->WriteDebugMessage("Cloning Entity\n");

	Entity* cloned = FACTORY->CreateEmptyEntity();
	//Entity* cloned = CORE->GetManager<EntityManager>()->CreateEmptyEntity();

	for (std::shared_ptr<Component> component : components_) {

		M_DEBUG->WriteDebugMessage("Begin to clone for single component\n");
		cloned->AddComponent(component->GetComponentTypeID(), component->Clone());
		M_DEBUG->WriteDebugMessage("Ended clone for single component\n");
	}

	return cloned;
}

// Destructs the entity through the factory
void Entity::Destroy() {
	//FACTORY->Destroy(this);
	CORE->GetManager<EntityManager>()->DeleteEntity(this);
}


// Function that converts input string from JSON into an equivalent enum
ComponentTypes StringToComponentType(const std::string str) {
	if (str == "Name")
		return ComponentTypes::NAME;
	else if (str == "Motion")
		return ComponentTypes::MOTION;
	else if (str == "Transform")
		return ComponentTypes::TRANSFORM;
	else if (str == "Health")
		return ComponentTypes::HEALTH;
	else if (str == "Camera")
		return ComponentTypes::CAMERA;
	else if (str == "Controller")
		return ComponentTypes::CONTROLLER;
	else if (str == "TextureRenderer")
		return ComponentTypes::TEXTURERENDERER;
	else if (str == "AnimationRenderer")
		return ComponentTypes::ANIMATIONRENDERER;
	else if (str == "TextRenderer")
		return ComponentTypes::TEXTRENDERER;
	else if (str == "AABB")
		return ComponentTypes::AABB;
	else if (str == "Scale")
		return ComponentTypes::SCALE;
	else if (str == "Status")
		return ComponentTypes::STATUS;
	else if (str == "PointLight")
		return ComponentTypes::POINTLIGHT;
	else if (str == "ConeLight")
		return ComponentTypes::CONELIGHT;
	else if (str == "BasicAI")
		return ComponentTypes::BASICAI;
	else if (str == "Clickable")
		return ComponentTypes::CLICKABLE;
	else if (str == "InputController")
		return ComponentTypes::INPUTCONTROLLER;
	else if (str == "AI")
		return ComponentTypes::AI;
	else
		return ComponentTypes::NONE;
}


