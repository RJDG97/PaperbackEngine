/**********************************************************************************
*\file         EntityManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Entity Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <set>
#include <map>
#include <memory>

#include "Entity/Entity.h"
#include "Manager/IManager.h"

class EntityManager : public IManager
{
public:
	using PlayerEntityVec = std::vector<Entity*>;
	using PlayerEntityVecIt = std::vector<Entity*>::iterator;

	using EntityIdMapType = std::map<EntityID, Entity*>;
	using EntityIdMapTypeIt = EntityIdMapType::iterator;

	using EntityIDSetDelete = std::set<Entity*>;
	using EntityIDSetDeleteIt = EntityIDSetDelete::iterator;

	using EntityArchetypeMapType = std::map<std::string, Entity*>;
	using EntityArchetypeMapTypeIt = EntityArchetypeMapType::iterator;

/******************************************************************************/
/*!
  \fn EntityManager()

  \brief Default constructor for EntityManager to initialize id
*/
/******************************************************************************/
	EntityManager();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initializes any required data members within the EntityManager
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn CreateEmptyEntity()

  \brief Creates an empty entity
*/
/******************************************************************************/
	Entity* CreateEmptyEntity();

/******************************************************************************/
/*!
  \fn CreateNewArchetype()

  \brief Create a new archetype and initialize it with a string
*/
/******************************************************************************/
	Entity* CreateNewArchetype(std::string new_archetype);

/******************************************************************************/
/*!
  \fn CloneEntity()

  \brief Clone an entity given an existing entity's ID
*/
/******************************************************************************/
	Entity* CloneEntity(EntityID id);

/******************************************************************************/
/*!
  \fn CloneArchetype()

  \brief Create a cloned entity with a base archetype
*/
/******************************************************************************/
	Entity* CloneArchetype(const std::string& archetype_name);

/******************************************************************************/
/*!
  \fn StoreEntityID()

  \brief Adds the created entity to the entity map within the EntityManager
*/
/******************************************************************************/
	void StoreEntityID(Entity* entity);


/******************************************************************************/
/*!
  \fn GetEntity()

  \brief Retrieves an Entity* with the coresponding entity id
*/
/******************************************************************************/
	Entity* GetEntity(EntityID id);

/******************************************************************************/
/*!
  \fn GetPlayerEntities()

  \brief Retrieves a vector of Entity* to players
*/
/******************************************************************************/
	std::vector<Entity*>& GetPlayerEntities();

/******************************************************************************/
/*!
  \fn AddNewArchetype()

  \brief Add an existing entity as a new archetype
*/
/******************************************************************************/
	void AddNewArchetype(std::string, Entity* archetype);

/******************************************************************************/
/*!
  \fn GetArchetype()

  \brief Retrieves an Entity* to an archetype with the archetype's name
*/
/******************************************************************************/
	Entity* GetArchetype(std::string archetype_name);

/******************************************************************************/
/*!
  \fn GetArchetypes()

  \brief Retrieve the archetype map
*/
/******************************************************************************/
	EntityArchetypeMapType& GetArchetypes();

/******************************************************************************/
/*!
  \fn GetEntities()

  \brief Retrieve the entity map
*/
/******************************************************************************/
	EntityIdMapType& GetEntities();
	

/******************************************************************************/
/*!
  \fn SortPlayerEntities()

  \brief Find Entity* to players and save them in a vector
*/
/******************************************************************************/
	void SortPlayerEntities();

/******************************************************************************/
/*!
  \fn DeleteEntity()

  \brief Delete an entity with coresponding entity id
*/
/******************************************************************************/
	void DeleteEntity(EntityID id);

/******************************************************************************/
/*!
  \fn DeleteEntity()

  \brief Delete an entity with the Entity*
*/
/******************************************************************************/
	void DeleteEntity(Entity* entity);

/******************************************************************************/
/*!
  \fn DeletePlayerEntities()

  \brief Delete all player entities from the vector of Entity* to players
*/
/******************************************************************************/
	void DeletePlayerEntities();

/******************************************************************************/
/*!
  \fn DeleteAllEntities()

  \brief Delete all existing entities
*/
/******************************************************************************/
	void DeleteAllEntities();

/******************************************************************************/
/*!
  \fn DeleteArchetype()

  \brief Delete a specific Entity* to an archetype
*/
/******************************************************************************/
	void DeleteArchetype(Entity* entity);

/******************************************************************************/
/*!
  \fn DeleteAllArchetype()

  \brief Initializes any required data members within the EntityManager
*/
/******************************************************************************/
	void DeleteAllArchetype();

/******************************************************************************/
/*!
  \fn UpdateEntityMap()

  \brief Remove all Entities that were marked to be deleted
*/
/******************************************************************************/
	void UpdateEntityMap();

/******************************************************************************/
/*!
  \fn UpdateArchetypeMap()

  \brief Remove all Archetypes that were marked to be deleted
*/
/******************************************************************************/
	void UpdateArchetypeMap();

private:
	size_t last_entity_id_;
	PlayerEntityVec player_list_;
	EntityIdMapType entity_id_map_;
	EntityIDSetDelete entities_to_delete_;
	EntityArchetypeMapType entity_archetype_map_;
	EntityIDSetDelete archetypes_to_delete_;
};

#endif