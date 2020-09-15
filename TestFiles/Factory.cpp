#include "Factory.h"
#include "Composition.h"
#include "ComponentCreator.h"

EntityFactory* FACTORY = NULL;

EntityFactory::EntityFactory() {

	FACTORY = this;
	_lastEntityId = 0;
}

EntityFactory::~EntityFactory() {

	ComponentMapType::iterator it = _componentMap.begin();

	//loops through and deleles every component creator
	for (; it != _componentMap.end(); ++it) {

		delete it->second;
	}
}

Entity* EntityFactory::create(const std::string& filename) {
	// Create the entity and initialize all components from file
	Entity* entity = BuildAndSerialize(filename);
	// If entity was successfully created, initialize
	if (entity) {
		entity->init();
	}
	return entity;
}


void EntityFactory::destroy(Entity* entity) {
	// Insert the entity into the set for deletion next update loop
	_objectsToDeleted.insert(entity);
}


void EntityFactory::update() {
	// Begin and end iterators
	EntityIt begin = _objectsToDeleted.begin();
	EntityIt end = _objectsToDeleted.end();

	// Loop through all entities that are to be removed and remove them
	for (; begin != end; ++begin) {
		Entity* entity = *begin;

		// Get entity's id
		EntityID id = entity->_objectID;
		// Check whether id exists within the map
		EntityIdMapTypeIt beginIt = _entityIdMap.find(id);

		// If it still exists, delete the entity
		if (beginIt != _entityIdMap.end()) {
			delete entity;
			_entityIdMap.erase(beginIt);
		}
	}
	// Clear and resize the set
	_objectsToDeleted.clear();
}

void EntityFactory::DestroyAllEntities() {
	
	EntityIdMapType::iterator it = _entityIdMap.begin();

	// Loop through all entities
	for (; it != _entityIdMap.end(); ++it) {
		// Delete all entities
		delete it->second;
	}
	// Clear and resize map
	_entityIdMap.clear();
}

Entity* EntityFactory::CreateEmptyEntity() {

	//creates entity w/ id
	//used for creating entities programmatically
	Entity* entity = new Entity();
	StoreEntityID(entity);
	return entity;
}

/*
Entity* EntityFactory::BuildAndSerialize(const std::string& filename) {
	
}*/

void EntityFactory::StoreEntityID(Entity* entity) {
	
	//increment counter and store entity into id map
	++_lastEntityId;
	entity->_objectID = _lastEntityId;

	_entityIdMap[_lastEntityId] = entity;
}

void EntityFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator) {
	
	//binds component creator to entry with component name
	_componentMap[name] = creator;
}

Entity* EntityFactory::GetObjectWithID(EntityID id) {
	
	EntityIdMapType::iterator it = _entityIdMap.find(id);

	// If entity id exists, return pointer to entity
	if (it != _entityIdMap.end()) {
		return it->second;
	}
	// Else, return null
	return NULL;
}