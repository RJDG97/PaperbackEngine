#include "Entity/Entity.h"
#include "Components/IComponent.h"
#include <algorithm>
#include "Systems/Factory.h"
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

	std::cout << "Initialising entity with type: " << static_cast<int>(entity_type_) << std::endl;

	for (EntityIt it = components_.begin(); it != components_.end(); ++it) {

		(*it)->owner_ = this;
		(*it)->Init();
	}
};

Entity::Entity() {
	// Initialise id to 0 since it will be assigned by factory
	object_id_ = 0;
	entity_type_ = EntityTypes::NONE;
}

// Destroys all components attached to an entity
Entity::~Entity() {
	
	components_.clear();
}

// Attach a new component to the entity and sort the components according to the order in 
// ComponentTypes.h
void Entity::AddComponent(ComponentTypes type_id, std::shared_ptr<Component> component) {

	//ensure pointer is not nullptr
	DEBUG_ASSERT(component, "Component is nullptr");

	component->type_id_ = type_id;
	components_.push_back(component);

	std::sort(components_.begin(), components_.end(), ComponentSorter);
}

// Returns a pointer to a component attached to an entity
std::shared_ptr<Component> Entity::GetComponent(ComponentTypes type_id) {
	
	EntityIt result = std::find_if(std::begin(components_), std::end(components_), std::bind(ComponentLocator, std::placeholders::_1, type_id));
	
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

//used for creating copies from a protoype/archetype
Entity* Entity::Clone() {

	Entity* cloned = FACTORY->CreateEmptyEntity();

	for (std::shared_ptr<Component> component : components_) {

		//cloned->AddComponent(component->GetComponentTypeID(), component->Clone());
	}

	cloned->Init();

	return cloned;
}

// Destructs the entity through the factory
void Entity::Destroy() {
	FACTORY->Destroy(this);
}