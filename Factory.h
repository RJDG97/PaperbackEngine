#pragma once
#include "ISystem.h"
#include "Entity.h"
#include <set>
#include <map>
#include <string>

class ComponentCreator;

class EntityFactory : public ISystem {

	//contains incrementally generated id
	unsigned int _lastEntityId;

	using ComponentMapType = std::map<std::string, ComponentCreator*>;
	ComponentMapType _componentMap;

	using EntityIdMapType = std::map<unsigned int, Entity*>;
	using EntityIdMapTypeIt = EntityIdMapType::iterator;
	EntityIdMapType _entityIdMap;

	using EntityIt = std::set<Entity*>::iterator;
	std::set<Entity*> _objectsToDeleted;

public:
	EntityFactory();
	~EntityFactory();

	//create, init & id new entity from file
	Entity* create(const std::string& filename);

	//adds entity to destroy list
	void destroy(Entity* entity);

	//updates factory and destroys dead entities
	virtual void update(float frametime);

	virtual std::string GetName() { return "Factory"; }

	// Used to destroy all entities in the game (Final shutdown)
	void DestroyAllEntities();

	// Creates and identifies an entity during run time
	// After components have been added, call Entity->init()
	Entity* CreateEmptyEntity();

	//builds composition & serialise from data file w/o initialising Entity
	//creates composition and modify data before initialisation
	Entity* BuildAndSerialize(const std::string& filename);

	// Identify the object and store it inside the enti map
	void StoreEntityID(Entity* entity);

	//adds component creator for data driven composition
	void AddComponentCreator(const std::string& name, ComponentCreator* creator);

	//returns an entity with the id
	//returns NULL if no longer existing
	Entity* GetObjectWithID(EntityID id);

	void SendMessageD(Message* msg);
};

extern EntityFactory* FACTORY;

