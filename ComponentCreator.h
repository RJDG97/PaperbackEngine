#pragma once
#include "ComponentTypes.h"

class Component;

class ComponentCreator {
public:
	ComponentTypes _typeId; // Might be able to make it private

	// Instantiate component with it's Type ID
	ComponentCreator(ComponentTypes typeId) : _typeId(typeId) 
	{}

	// Create a component
	virtual Component* create() = 0;

	virtual ~ComponentCreator() = default;
};


template <typename type>
class ComponentCreatorType : public ComponentCreator {
public:
	// Instantiate component with it's Type ID
	ComponentCreatorType(ComponentTypes id) : ComponentCreator(id) 
	{}

	// Returns a new instance of a component that it is templated for
	virtual Component* create() {
		return new type();
	}
};