#pragma once
#include "ComponentTypes.h"
class Component;

class ComponentCreator {
public:

	ComponentTypes _typeId;

	ComponentCreator(ComponentTypes typeId) : _typeId(typeId) {}
};


template <typename type>
class ComponentCreatorType : ComponentCreator {
public:
	ComponentCreatorType(ComponentTypes id) : ComponentCreator(id) 
	{}

	virtual Component* create() {
		return new type();
	}
};