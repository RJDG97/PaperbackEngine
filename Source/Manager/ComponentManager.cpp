#include "Systems/Debug.h"
#include "Manager/ComponentManager.h"


template <typename T>
void CMap<T>::AddComponent(EntityID id, T* component) {
	
	DEBUG_ASSERT(component_map_.find(id) == component_map_.end(), "Component already exists for entity!");
	component_map_.emplace(id, component);
}

template <typename T>
void CMap<T>::RemoveComponent(EntityID id) {
	
	DEBUG_ASSERT(component_map_.find(id) == component_map_.end(), "Component does not exist for entity!");
	component_map_.erase(id);
}

template <typename T>
T& CMap<T>::GetComponent(EntityID id) {

	DEBUG_ASSERT(component_map_.find(id) == component_map_.end(), "Component already exists for entity!");
	return component_map_[id];
}

template <typename T>
CMap<T>::~CMap() {

	component_map_.clear();
}



template <typename... Ts>
CManager<Ts...>::CManager() {
	// Possibly something uwuz
}

template <typename... Ts>
void CManager<Ts...>::Init() {
	// Add component creators here
}

template <typename... Ts>
void CManager<Ts...>::AddComponentCreator(std::string name, IComponentCreator* creator) {

	// If component creator already exists, throw exception
	DEBUG_ASSERT(component_creator_arr_.find(name) != component_creator_arr_.end(), "Component creator already exists");

	component_creator_arr_[name] = creator;
}

template <typename... Ts>
IComponentCreator* CManager<Ts...>::GetComponentCreator(std::string name) {

	// If component creator already exists, return it
	if (component_creator_arr_.find(name) != component_creator_arr_.end()) {
		return component_creator_arr_[name];
	}

	return nullptr;
}

template <typename... Ts>
template <typename T>
T& CManager<Ts...>::GetComponent(EntityID id) {
	return CMap<T>::GetComponent(id);
}

template <typename... Ts>
template <typename T>
T& CManager<Ts...>::GetComponentArray() {
	return CMap<T>;
}

template <typename... Ts>
template <typename T>
void CManager<Ts...>::AddComponent(EntityID id, T* component) {
	CMap<T>::AddComponent(id, component);
}

template <typename... Ts>
template <typename T>
void CManager<Ts...>::RemoveComponent(EntityID id) {
	CMap<T>::RemoveComponent(id);
}

template <typename... Ts>
CManager<Ts...>::~CManager() {

	ComponentMapType::iterator it = component_creator_arr_.begin();

	//loops through and deleles every component creator
	for (; it != component_creator_arr_.end(); ++it) {

		delete it->second;
	}
}