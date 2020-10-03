#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Components/IComponent.h"
#include <vector>

enum class EntityTypes;
using EntityID = size_t;
using ComponentArr = std::vector<Component*>;

// defines a single entity that owns components
// aka gameobject in Unity terms
class Entity {
	using EntityIt = std::vector<Component*>::iterator;

	// Vector of components attached to the entity
	ComponentArr components_;
	// Unique ID of an entity (Begins at 1)
	EntityID object_id_;
	EntityTypes entity_type_;

	// For use in FACTORY only
	Entity();
	~Entity();
public:

	friend class EntityFactory;

	//returns component by id
	//NULL if invalid
	Component* GetComponent(ComponentTypes typeId);

	//guaranteed method of getting component
	template <typename return_type>
	return_type* GetComponentType(ComponentTypes id);

	// For initializing all components attached to the entity
	void Init();

	// For properly destroying an entity by adding it to the 
	// factory's destroy list (Destroyed in next game loop)
	void Destroy();

	// Adds a component to the current entity
	void AddComponent(ComponentTypes typeId, Component* component);

	EntityID GetID() { return object_id_; };

	EntityTypes GetType() { return entity_type_; };

	Entity* Clone();
};

enum class EntityTypes {

	None = 0,
	Player,
	Enemy,
	Wall,
	StaticObj
};

#endif