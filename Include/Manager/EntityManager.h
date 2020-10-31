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

	using EntityIdMapType = std::map<EntityID, Entity*>;
	using EntityIdMapTypeIt = EntityIdMapType::iterator;

	using EntityIDSetDelete = std::set<Entity*>;
	using EntityIDSetDeleteIt = EntityIDSetDelete::iterator;

	using EntityArchetypeMapType = std::map<std::string, Entity*>;
	using EntityArchetypeMapTypeIt = EntityArchetypeMapType::iterator;


	// Functions
	EntityManager();

	void Init() override;

	Entity* CreateEmptyEntity();
	Entity* CreateNewArchetype(/*possible parameters*/);
	Entity* CloneEntity(EntityID id);
	Entity* CloneArchetype(const std::string& archetype_name);
	void StoreEntityID(Entity* entity);

	Entity* GetEntity(EntityID id);
	void AddNewArchetype(std::string, Entity* archetype);
	Entity* GetArchetype(std::string archetype_name);
	EntityArchetypeMapType& GetArchetypes();
	EntityIdMapType& GetEntities();

	void DeleteEntity(EntityID id);
	void DeleteEntity(Entity* entity);
	void DeleteAllEntities();
	void DeleteAllArchetype();

	void UpdateEntityMap();

private:
	size_t last_entity_id_;
	EntityIdMapType entity_id_map_;
	EntityIDSetDelete entities_to_delete_;
	EntityArchetypeMapType entity_archetype_map_;
};

#endif