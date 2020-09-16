#include "Composition.h"
#include "IComponent.h"
#include <algorithm>
#include "Factory.h"

//Used to sort components using their type Id.
struct ComponentSorter
{
	bool operator()(Component* left, Component* right) const
	{
		return left->_typeId < right->_typeId;
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
		if(components[mid]->_typeId < name)
			begin = mid + 1;
		else
			end = mid;
	}

	if((begin < components.size()) && (components[begin]->_typeId == name))
		return components[begin];
	else
		return NULL;
}

void Entity::init() {

	//inits all components owned by entity and set the component's owner
	//allows each component to be initialised separate from ctor

	for (EntityIt it = _components.begin(); it != _components.end(); ++it) {

		(*it)->_owner = this;
		(*it)->init();
	}
};

Entity::Entity() {

	_objectID = 0;
}

Entity::~Entity() {
	EntityIt begin = _components.begin();
	EntityIt end = _components.end();
	for (; begin < end; ++begin) {
		delete* begin;
	}
}

void Entity::AddComponent(ComponentTypes typeId, Component* component) {

	component->_typeId = typeId;
	_components.push_back(component);

	std::sort(_components.begin(), _components.end(), ComponentSorter());
}


Component* Entity::GetComponent(ComponentTypes typeId) {
	
	return BinaryComponentSearch(_components, typeId);
}

void Entity::destroy() {
	FACTORY->destroy(this);
}