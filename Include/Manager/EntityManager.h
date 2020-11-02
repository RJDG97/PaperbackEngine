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


	// Functions
	EntityManager();

	void Init() override;

	Entity* CreateEmptyEntity();
	Entity* CreateNewArchetype(std::string new_archetype);
	Entity* CloneEntity(EntityID id);
	Entity* CloneArchetype(const std::string& archetype_name);
	void StoreEntityID(Entity* entity);

	Entity* GetEntity(EntityID id);
	std::vector<Entity*>& GetPlayerEntities();
	void AddNewArchetype(std::string, Entity* archetype);
	Entity* GetArchetype(std::string archetype_name);
	EntityArchetypeMapType& GetArchetypes();
	EntityIdMapType& GetEntities();
	
	void SortPlayerEntities();
	void DeleteEntity(EntityID id);
	void DeleteEntity(Entity* entity);
	void DeletePlayerEntities();
	void DeleteAllEntities();
	void DeleteAllArchetype();

	void UpdateEntityMap();

private:
	size_t last_entity_id_;
	PlayerEntityVec player_list_;
	EntityIdMapType entity_id_map_;
	EntityIDSetDelete entities_to_delete_;
	EntityArchetypeMapType entity_archetype_map_;
};

#endif