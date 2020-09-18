#include "Entity.h"
#include "IComponent.h"
#include <algorithm>
#include "Factory.h"

//Used to sort components using their type Id.
struct ComponentSorter
{
	bool operator()(Component* left, Component* right) const
	{
		return left->GetComponentType() < right->GetComponentType();
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
		if(components[mid]->GetComponentType() < name)
			begin = mid + 1;
		else
			end = mid;
	}

	if((begin < components.size()) && (components[begin]->GetComponentType() == name))
		return components[begin];
	else
		return NULL;
}

void Entity::init() {

	//inits all components owned by entity and set the component's owner
	//allows each component to be initialised separate from ctor

	for (EntityIt it = _components.begin(); it != _components.end(); ++it) {

		(*it)->owner_ = this;
		(*it)->init();
	}
};

Entity::Entity() {

	// Initialise id to 0 since it will be assigned by factory
	_objectID = 0;
}

// Destroys all components attached to an entity
Entity::~Entity() {
	EntityIt begin = _components.begin();
	EntityIt end = _components.end();
	for (; begin < end; ++begin) {
		delete* begin;
	}
}

// Attach a new component to the entity and sort the components according to the order in 
// ComponentTypes.h
void Entity::AddComponent(ComponentTypes typeId, Component* component) {

	component->type_id_ = typeId;
	_components.push_back(component);

	std::sort(_components.begin(), _components.end(), ComponentSorter());
}

// Returns a pointer to a component attached to an entity
Component* Entity::GetComponent(ComponentTypes typeId) {
	
	return BinaryComponentSearch(_components, typeId);
}

// Destructs the entity through the factory
void Entity::destroy() {
	FACTORY->destroy(this);
}