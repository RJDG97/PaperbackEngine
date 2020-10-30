#pragma once
#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "Systems/ISystem.h"
#include "Entity/Entity.h"
#include "GameStates/Levels.h"
#include <set>
#include <map>
#include <string>

class IComponentCreator;

class EntityFactory : public ISystem {

	bool debug_;

	//contains incrementally generated id
	unsigned int last_entity_id_;

	// 
	using ComponentMapType = std::map<std::string, IComponentCreator*>;
	ComponentMapType component_map_;

	// 
	using EntityIdMapType = std::map<EntityID, Entity*>;
	using EntityIdMapTypeIt = EntityIdMapType::iterator;
	EntityIdMapType entity_id_map_;

	// 
	using EntityArchetypeMapType = std::map<std::string, Entity*>;
	using EntityArchetypeMapTypeIt = EntityArchetypeMapType::iterator;
	EntityArchetypeMapType entity_archetype_map_;

	// For storing entities that are to be deleted in update loop
	using EntityIt = std::set<Entity*>::iterator;
	std::set<Entity*> objects_to_delete;

	//will contain the additional details about what files are loaded
	//possibly shifted to entity manager if need be
	Levels levels_;

/******************************************************************************/
/*!
  \fn CloneLevelEntities()

  \brief Creates and serializes entities of an archetype in a level given 
		 an archetype name and json to load from
*/
/******************************************************************************/
	void CloneLevelEntities(const std::string& filename, const std::string& archetype_name);

public:
/******************************************************************************/
/*!
  \fn EntityFactory()

  \brief Constructor for EntityFactory that initialises data members
*/
/******************************************************************************/
	EntityFactory();

/******************************************************************************/
/*!
  \fn ~EntityFactory()

  \brief Destructor for EntityFactory that ensures that the component creator
		 container is cleaned up
*/
/******************************************************************************/
	~EntityFactory();

/******************************************************************************/
/*!
  \fn Create()

  \brief Creates a new entity and initialises it from a JSON file
*/
/******************************************************************************/
	Entity* CloneArchetype(const std::string& archetype_name);

/******************************************************************************/
/*!
  \fn CreateAllArchetypes()

  \brief Creates and serializes all archetypes from a JSON
*/
/******************************************************************************/
	void CreateAllArchetypes(const std::string& filename);
	
/******************************************************************************/
/*!
  \fn DeSerializeLevelEntities()

  \brief Deserializes all entities in a level by by loading their component
		 values from a JSON file
*/
/******************************************************************************/
	void DeSerializeLevelEntities(const std::string& filename);

/******************************************************************************/
/*!
  \fn SerializeArchetypes()

  \brief Serializes all archetypes by by saving their component
		 values within a JSON file
*/
/******************************************************************************/
	void SerializeArchetypes(const std::string& filename);
	
/******************************************************************************/
/*!
  \fn SerializeCurrentLevelEntities()

  \brief Serializes all entities in a level by by saving their component
		 values within multiple JSON files
*/
/******************************************************************************/
	void SerializeCurrentLevelEntities();

/******************************************************************************/
/*!
  \fn SerializeLevelPaths()

  \brief Serializes all paths in the levels struct into the currently set 
		 parameters
*/
/******************************************************************************/
	void SerializeLevelPaths();

/******************************************************************************/
/*!
  \fn Destroy()

  \brief Marks the entity to be destroyed within the next update loop
*/
/******************************************************************************/
	void Destroy(Entity* entity);

/******************************************************************************/
/*!
  \fn Update()

  \brief Removes entities that are marked to be deleted
*/
/******************************************************************************/
	virtual void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() override { return "Factory"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Handles incoming messages and sorts based on message id
*/
/******************************************************************************/
	void SendMessageD(Message* msg) override;

/******************************************************************************/
/*!
  \fn DestroyAllEntities()

  \brief Used to destroy all active entities (Possibly when shutting down game)
*/
/******************************************************************************/
	void DestroyAllEntities();

/******************************************************************************/
/*!
  \fn DestroyAllEntities()

  \brief Used to destroy all archetypes
*/
/******************************************************************************/
	void DestroyAllArchetypes();

/******************************************************************************/
/*!
  \fn CreateEmptyEntity()

  \brief Used to create an empty entity
*/
/******************************************************************************/
	Entity* CreateEmptyEntity();

/******************************************************************************/
/*!
  \fn StoreEntityID()

  \brief Identify the entity's ID and store it within the entity map
*/
/******************************************************************************/
	void StoreEntityID(Entity* entity);

/******************************************************************************/
/*!
  \fn AddComponentCreator()

  \brief Used to add component creator to a map for ease of contructing new
		 components during serialization
*/
/******************************************************************************/
	void AddComponentCreator(const std::string& name, IComponentCreator* creator);

	
/******************************************************************************/
/*!
  \fn GetLevelPath()

  \brief Used to retrieve a filepath to a specific level json. 
*/
/******************************************************************************/
	std::string GetLevelPath(const std::string& name);

/******************************************************************************/
/*!
  \fn GetLevel()

  \brief Used to retrieve a level struct for a specific level json.
*/
/******************************************************************************/
	Level* GetLevel(const std::string& name);

/******************************************************************************/
/*!
  \fn GetLevelsFile()

  \brief Used to retrieve a levels struct containing paths to multiple level 
		 jsons
*/
/******************************************************************************/
	Levels* GetLevelsFile();

/******************************************************************************/
/*!
  \fn Init()

  \brief Used to initialise a system if required
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn GetObjectWithID()

  \brief Returns a pointer to the entity with the given unique ID
		 Returns nullptr if the entity does not exist
*/
/******************************************************************************/
	Entity* GetObjectWithID(EntityID id);
};

extern EntityFactory* FACTORY;

/******************************************************************************/
/*!
  \fn DeSerializeJSON()

  \brief Loads the content of a JSON file into a doc
*/
/******************************************************************************/
void DeSerializeJSON(const std::string& filename, rapidjson::Document& doc);

#endif