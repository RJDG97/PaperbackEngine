#include "ComponentTypes.h"
#include "ISerializer.h"
#include "lib/rapidjson/filereadstream.h"
#include "lib/rapidjson/document.h"

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//forward declaration of entity class
//will be inherited by file containing Entity definition
class Entity;

class Component {

	// Pointer to the entity that owns the component
	Entity* owner_;
	ComponentTypes type_id_;

public:

	friend class Entity;

	// Inits the component ingame 
	virtual void Init() {};

	// Reads and initializes the component's data members from a stream
	virtual void Serialize(ISerializer& str){};

	// Returns the pointer to the entity that owns the component
	Entity* GetOwner() { return owner_; }

	//returns type id
	ComponentTypes GetComponentTypeID() { return type_id_; }

protected:
	// Double check why protected
	virtual ~Component() = default;
};

#endif