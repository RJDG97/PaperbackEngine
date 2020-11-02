#include "Systems/Debug.h"
#include "Engine/Core.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"

EntityManager::EntityManager() :
	last_entity_id_{}
{	}

void EntityManager::Init() {
	
}

Entity* EntityManager::CreateEmptyEntity() {

	Entity* entity =  new Entity();
	StoreEntityID(entity);
	return entity;
}

Entity* EntityManager::CreateNewArchetype() {

	// Double check this implementation
	return CreateEmptyEntity();
}

Entity* EntityManager::CloneEntity(EntityID id) {

	EntityIdMapTypeIt it = entity_id_map_.find(id);
	DEBUG_ASSERT(it != entity_id_map_.end(), "Entity does not exist!");

	M_DEBUG->WriteDebugMessage("Preparing to clone and init entity\n");
	Entity* cloned = it->second->Clone();
	cloned->Init();

	return cloned;
}

Entity* EntityManager::CloneArchetype(const std::string& archetype_name) {
	
	EntityArchetypeMapTypeIt it = entity_archetype_map_.find(archetype_name);
	DEBUG_ASSERT((it != entity_archetype_map_.end()), "Archetype was not found!");

	M_DEBUG->WriteDebugMessage("Preparing to clone and init archetype\n");
	Entity* cloned = it->second->Clone();
	cloned->Init();

	return cloned;
}

void EntityManager::StoreEntityID(Entity* entity) {

	entity->object_id_ = ++last_entity_id_;
	entity_id_map_[last_entity_id_] = entity;

	M_DEBUG->WriteDebugMessage("Storing entity with ID: " + std::to_string(last_entity_id_) + "\n");

}

Entity* EntityManager::GetEntity(EntityID id) {

	EntityIdMapType::iterator it = entity_id_map_.find(id);
	
	if (it != entity_id_map_.end()) {
		return it->second;
	}
	return nullptr;
}

std::vector<Entity*>& EntityManager::GetPlayerEntities() {
	
	return player_list_;
}

void EntityManager::AddNewArchetype(std::string archetype_name, Entity* archetype) {
	
	entity_archetype_map_[archetype_name] = archetype;
}

Entity* EntityManager::GetArchetype(std::string archetype_name) {

	DEBUG_ASSERT((entity_archetype_map_.find(archetype_name) != entity_archetype_map_.end()), "Archetype was not found!");
	return entity_archetype_map_[archetype_name];
}

EntityManager::EntityArchetypeMapType& EntityManager::GetArchetypes() {
	return entity_archetype_map_;
}

EntityManager::EntityIdMapType& EntityManager::GetEntities() {

	return entity_id_map_;
}

void EntityManager::DeleteEntity(EntityID id) {

	EntityIdMapTypeIt it = entity_id_map_.find(id);
	DEBUG_ASSERT(it != entity_id_map_.end(), "Entity does not exist");

	entities_to_delete_.insert(it->second);
}

void EntityManager::DeleteEntity(Entity* entity) {

	entities_to_delete_.insert(entity);
}

void EntityManager::DeletePlayerEntities() {

	player_list_.clear();
}

void EntityManager::DeleteAllEntities() {

	M_DEBUG->WriteDebugMessage("Deleting all entities!\n");
	EntityIdMapType::iterator it = entity_id_map_.begin();

	for (; it != entity_id_map_.end(); ++it) {
		// Log id of entity that is being deleted
		M_DEBUG->WriteDebugMessage("Deleting entity id: " + std::to_string(it->first) + "\n");
		// Delete all entities
		delete it->second;
	}

	// Clear and resize map
	entity_id_map_.clear();
}

void EntityManager::DeleteAllArchetype() {

	for (EntityArchetypeMapTypeIt it2 = entity_archetype_map_.begin(); it2 != entity_archetype_map_.end(); ++it2) {
		// Log EntityType of entity that is being deleted (Archetype)
		std::stringstream str;
		str << "Deleting entity of type: " << typeid(it2->first).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());
		// Delete all entities
		delete it2->second;
	}

	entity_archetype_map_.clear();
}

void EntityManager::UpdateEntityMap() {

	for (EntityIDSetDeleteIt begin = entities_to_delete_.begin(); begin != entities_to_delete_.end(); ++begin) {

		// Assign to blank Entity
		Entity* entity = *begin;
		EntityID id = entity->object_id_;

		// Check if entity still exists
		EntityIdMapTypeIt check_it = entity_id_map_.find(id);

		if (check_it != entity_id_map_.end()) {
			M_DEBUG->WriteDebugMessage("Deleting entity id: " + std::to_string(check_it->first) + "\n");
			delete entity;
			entity_id_map_.erase(check_it);
		}
	}

	entities_to_delete_.clear();
}

// Private helper function to search for player entities
void EntityManager::SortPlayerEntities() {
	
	ComponentManager* manager = &*CORE->GetManager<ComponentManager>();
	CMap<Name>* name = manager->GetComponentArray<Name>();

	for (CMap<Name>::MapTypeIt it = name->begin(); it != name->end(); ++it) {
		
		if (it->second->GetEntityName() == "Player") {
			player_list_.push_back(it->second->GetOwner());
		}
	}
}