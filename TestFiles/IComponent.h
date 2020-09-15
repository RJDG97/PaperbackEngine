#include "ComponentTypes.h"

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//forward declaration of entity class
//will be inherited by file containing Entity definition
class Entity;

class Component {

	// Pointer to the entity that owns the component
	Entity* _owner;

public:

	ComponentTypes _typeId;

	friend class Entity;

	// Inits the component ingame 
	virtual void init() {};

	// Reads and initializes the component's data members from a stream
	//virtual void serialize(ISerialiser& str){};

	// Returns the pointer to the entity that owns the component
	Entity* GetOwner() { return _owner; }

protected:
	// Double check why protected
	virtual ~Component() = default;
};

#endif