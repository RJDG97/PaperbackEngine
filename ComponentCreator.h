#pragma once
#include "ComponentTypes.h"

class Component;

class ComponentCreator {

	ComponentTypes type_id_;
public:

	// Instantiate component with it's Type ID
	ComponentCreator(ComponentTypes type_id) : type_id_(type_id)
	{}

	ComponentTypes GetComponentTypeID() const { return type_id_; }

	// Create a component
	virtual Component* Create() = 0;

	virtual ~ComponentCreator() = default;
};


template <typename type>
class ComponentCreatorType : public ComponentCreator {
public:
	// Instantiate component with it's Type ID
	ComponentCreatorType(ComponentTypes id) : ComponentCreator(id) 
	{}

	// Returns a new instance of a component that it is templated for
	virtual Component* Create() {
		return new type();
	}
};