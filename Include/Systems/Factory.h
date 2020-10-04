#pragma once
#include "Systems/ISystem.h"
#include "Entity/Entity.h"
#include <set>
#include <map>
#include <string>

class ComponentCreator;

class EntityFactory : public ISystem {

	bool debug_;

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

/******************************************************************************/
/*!
  \fn TestBuild()

  \brief Used for creating fixed entities w/ predetermined components that do
		 not read from the JSON files
*/
/******************************************************************************/
	Entity* TestBuild();

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

	//create, init & id new entity from file
/******************************************************************************/
/*!
  \fn Create()

  \brief Creates a new entity and initialises it from a JSON file
*/
/******************************************************************************/
	Entity* Create(const std::string& filename);

/******************************************************************************/
/*!
  \fn CreateAndSerializeArchetype()

  \brief Creates a new entity archetype and initialises it from a JSON file
*/
/******************************************************************************/
	Entity* CreateAndSerializeArchetype(const std::string& filename, const std::string& entity_name, EntityTypes id);

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
	void AddComponentCreator(const std::string& name, ComponentCreator* creator);

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

