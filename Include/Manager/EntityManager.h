#pragma once
#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <set>
#include <map>

#include "Entity/Entity.h"
#include "Manager/IManager.h"

class EntityManager : public IManager
{
public:

	using EntityIdMapType = std::map<EntityID, Entity*>;
	using EntityIdMapTypeIt = EntityIdMapType::iterator;

	using EntityArchetypeMapType = std::map<std::string, Entity*>;
	using EntityArchetypeMapTypeIt = EntityArchetypeMapType::iterator;

	using EntityIt = std::set<Entity*>::iterator;


	// Functions
	Entity* CreateEmptyEntity();
	Entity* CreateNewArchetype(/*possible parameters*/);
	Entity* CloneEntity(/*EntityID id*/);
	Entity* CloneArchetype(std::string& archetype_name);
	void StoreEntityID(Entity* entity);

	Entity* GetArchetype(std::string archetype_name);
	EntityIdMapType& GetEntities();

	void DeleteEntity(EntityID id);
	void DeleteAllEntities();
	void DeleteAllArchetype();

private:

	EntityIdMapType entity_id_map_;
	std::set<Entity*> objects_to_delete;
	EntityArchetypeMapType entity_archetype_map_;
};

#endif