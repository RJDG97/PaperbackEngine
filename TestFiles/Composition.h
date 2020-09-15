#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "IComponent.h"
#include <vector>

using EntityID = unsigned int;

using ComponentArr = std::vector<Component*>;

class Entity {
	using EntityIt = std::vector<Component*>::iterator;

	ComponentArr _components;
	EntityID _objectID;

	// For use in FACTORY only
	Entity();
	~Entity();
public:

	friend class EntityFactory;

	//returns component by id
	//NULL if invalid
	Component* GetComponent(ComponentTypes typeId);

	//guaranteed method of getting component
	/*template <typename returnType>
	returnType* GetComponentType(int id); // replace id w/ enum
	*/

	// For initializing all components attached to the entity
	void init();

	// For properly destroying an entity by adding it to the 
	// factory's destroy list (Destroyed in next game loop)
	void destroy();

	// Adds a component to the current entity
	void AddComponent(ComponentTypes typeId, Component* component);

	EntityID GetID() { return _objectID; };
};
/*
//A more advanced type safe way of accessing components.
//Interface becomes Transform* transform = object->has(Transform);
template<typename returnType>
returnType * GameObjectComposition::GetComponentType(int typeId) // replace id w/ enum
{
	return static_cast<returnType*>( GetComponent(typeId) );
}*/

#endif