#pragma once
#include "Systems/ISystem.h"
#include "Entity/Entity.h"
#include <set>
#include <map>
#include <string>

class ComponentCreator;

class EntityFactory : public ISystem {

	//contains incrementally generated id
	unsigned int last_entity_id_;

	// 
	using ComponentMapType = std::map<std::string, ComponentCreator*>;
	ComponentMapType component_map_;

	// 
	using EntityIdMapType = std::map<EntityID, Entity*>;
	using EntityIdMapTypeIt = EntityIdMapType::iterator;
	EntityIdMapType entity_id_map_;

	// 
	using EntityArchetypeMapType = std::map<EntityTypes, Entity*>;
	using EntityArchetypeMapTypeIt = EntityArchetypeMapType::iterator;
	EntityArchetypeMapType entity_archetype_map_;

	// For storing entities that are to be deleted in update loop
	using EntityIt = std::set<Entity*>::iterator;
	std::set<Entity*> objects_to_delete;

	//used for creating fixed entities w/ predetermined components
	Entity* TestBuild();

public:
	EntityFactory();
	~EntityFactory();

	//create, init & id new entity from file
	Entity* Create(const std::string& filename);

	//creates new archetype and adds to map of archetypes
	Entity* CreateAndSerializeArchetype(const std::string& filename, const std::string& entity_name, EntityTypes id);

	//adds entity to destroy list
	void Destroy(Entity* entity);

	//updates factory and destroys dead entities
	virtual void Update(float frametime) override;

	virtual std::string GetName() override { return "Factory"; }

	void SendMessageD(Message* msg) override;

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

	void Init() override;

	//returns an entity with the id
	//returns NULL if no longer existing
	Entity* GetObjectWithID(EntityID id);
};

extern EntityFactory* FACTORY;

