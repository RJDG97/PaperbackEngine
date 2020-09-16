#pragma once
#include "ComponentTypes.h"
class Component;

class ComponentCreator {
public:

	ComponentTypes _typeId;

	ComponentCreator(ComponentTypes typeId) : _typeId(typeId) {}

	virtual Component* create() = 0;
	virtual ~ComponentCreator() = default;
};


template <typename type>
class ComponentCreatorType : public ComponentCreator {
public:
	ComponentCreatorType(ComponentTypes id) : ComponentCreator(id) 
	{}

	virtual Component* create() {
		return new type();
	}
};