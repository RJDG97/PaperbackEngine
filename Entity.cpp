#include "Entity.h"
#include "IComponent.h"
#include <algorithm>
#include "Factory.h"
#include <iostream>

//Used to sort components using their type Id.
struct ComponentSorter
{
	bool operator()(Component* left, Component* right) const
	{
		return left->GetComponentTypeID() < right->GetComponentTypeID();
	}
};

//Binary search a sorted array of components.
Component* BinaryComponentSearch(ComponentArr& components, ComponentTypes name)
{
	size_t begin = 0;
	size_t end = components.size();

	while(begin < end)
	{
		size_t mid = (begin+end) / 2;
		if(components[mid]->GetComponentTypeID() < name)
			begin = mid + 1;
		else
			end = mid;
	}

	if((begin < components.size()) && (components[begin]->GetComponentTypeID() == name))
		return components[begin];
	else
		return NULL;
}

int counter = 0;

void Entity::Init() {

	//inits all components owned by entity and set the component's owner
	//allows each component to be initialised separate from ctor

	//temporary method to alternate tags
	//should be offload to build&serialize
	{
		entity_type_ = static_cast<EntityTypes>(++counter);

		if (counter > 1)
			counter = 0;
	}

	std::cout << "Initialising entity with type: " << static_cast<int>(entity_type_) << std::endl;

	for (EntityIt it = components_.begin(); it != components_.end(); ++it) {

		(*it)->owner_ = this;
		(*it)->Init();
	}
};

Entity::Entity() {
	// Initialise id to 0 since it will be assigned by factory
	object_id_ = 0;
	entity_type_ = EntityTypes::None;
}

// Destroys all components attached to an entity
Entity::~Entity() {
	EntityIt begin = components_.begin();
	EntityIt end = components_.end();
	for (; begin < end; ++begin) {
		delete* begin;
	}
}

// Attach a new component to the entity and sort the components according to the order in 
// ComponentTypes.h
void Entity::AddComponent(ComponentTypes typeId, Component* component) {

	//ensure pointer is not nullptr
	assert(component);

	component->type_id_ = typeId;
	components_.push_back(component);

	std::sort(components_.begin(), components_.end(), ComponentSorter());
}

// Returns a pointer to a component attached to an entity
Component* Entity::GetComponent(ComponentTypes typeId) {
	
	return BinaryComponentSearch(components_, typeId);
}

//A more advanced type safe way of accessing components.
//Interface becomes Transform* transform = object->has(Transform);
template<typename return_type>
return_type* Entity::GetComponentType(ComponentTypes typeId)
{
	return static_cast<return_type*>(GetComponent(typeId));
}

//used for creating copies from a protoype/archetype
Entity* Entity::Clone() {

	Entity* cloned = FACTORY->CreateEmptyEntity();

	for (Component* component : components_) {

		cloned->AddComponent(component->GetComponentTypeID(), component->Clone());
	}

	cloned->Init();

	return cloned;
}

// Destructs the entity through the factory
void Entity::Destroy() {
	FACTORY->Destroy(this);
}