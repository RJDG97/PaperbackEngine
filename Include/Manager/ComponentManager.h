#pragma once
#ifndef _COMPONENT_MANAGER_H_
#define _COMPONENT_MANAGER_H_

#include <map>
#include <string>
#include <unordered_map>

#include "Systems/Debug.h"

#include "Manager/IManager.h"
#include "Entity/ComponentCreator.h"

#include "Components/Name.h"
#include "Components/Scale.h"
#include "Components/Status.h"
#include "Components/Health.h"
#include "Components/PointLight.h"
#include "Components/AABB.h"
#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/BasicAI.h"
#include "Components/TextureRenderer.h"
#include "Components/AnimationRenderer.h"
#include "Components/TextRenderer.h"
#include "Components/Clickable.h"
#include "Components/InputController.h"
#include "Components/AI.h"

template <typename T>
class CMap
{
public:
	using MapType = std::unordered_map<EntityID, T*>;
	using MapTypeIt = typename MapType::iterator;
	MapTypeIt begin();
	MapTypeIt end();
	void AddComponent(EntityID id, T* component);
	void RemoveComponent(EntityID id);
	T* GetComponent(EntityID id);
	~CMap();
private:

	MapType component_map_;
};


// Original implementation thoughts
/*
using ComponentMap = std::tuple<
	CMap<Name>,
	CMap<Scale>,
	CMap<Status>,
	CMap<Health>,
	CMap<PointLight>,
	CMap<AABB>,
	CMap<Transform>,
	CMap<Motion>,
	CMap<BasicAI>,
	CMap<TextureRenderer>,
	CMap<AnimationRenderer>,
	CMap<TextRenderer>,
	CMap<Clickable>,
	CMap<InputController>
>;
*/

template <typename... Ts>
class CManager : public IManager, private CMap<Ts>...
{
public:

	// Component Creator
	using ComponentMapType = std::unordered_map<std::string, IComponentCreator*>;
	using ComponentMapTypeIt = std::unordered_map<std::string, IComponentCreator*>::iterator;

	/*
	// Components
	using NameMapType = std::unordered_map<EntityID, Name*>;
	using NameMapTypeIt = std::unordered_map<EntityID, Name*>::iterator;

	using AABBMapType = std::unordered_map<EntityID, AABB*>;
	using AABBMapTypeIt = std::unordered_map<EntityID, AABB*>::iterator;

	using ScaleMapType = std::unordered_map<EntityID, Scale*>;
	using ScaleMapTypeIt = std::unordered_map<EntityID, Scale*>::iterator;

	using StatusMapType = std::unordered_map<EntityID, Status*>;
	using StatusMapTypeIt = std::unordered_map<EntityID, Status*>::iterator;

	using HealthMapType = std::unordered_map<EntityID, Health*>;
	using HealthMapTypeIt = std::unordered_map<EntityID, Health*>::iterator;

	using MotionMapType = std::unordered_map<EntityID, Motion*>;
	using MotionMapTypeIt = std::unordered_map<EntityID, Motion*>::iterator;

	using BasicAIMapType = std::unordered_map<EntityID, BasicAI*>;
	using BasicAIMapTypeIt = std::unordered_map<EntityID, BasicAI*>::iterator;

	using CliclableMapType = std::unordered_map<EntityID, Clickable*>;
	using ClickableMapTypeIt = std::unordered_map<EntityID, Clickable*>::iterator;

	using TransformMapType = std::unordered_map<EntityID, Transform*>;
	using TransformMapTypeIt = std::unordered_map<EntityID, Transform*>::iterator;

	using PointLightMapType = std::unordered_map<EntityID, PointLight*>;
	using PointLightMapTypeIt = std::unordered_map<EntityID, PointLight*>::iterator;

	using TextRendererMapType = std::unordered_map<EntityID, TextRenderer*>;
	using TextRendererMapTypeIt = std::unordered_map<EntityID, TextRenderer*>::iterator;

	using InputControllerMapType = std::unordered_map<EntityID, InputController*>;
	using InputControllerMapTypeIt = std::unordered_map<EntityID, InputController*>::iterator;

	using TextureRendererMapType = std::unordered_map<EntityID, TextureRenderer*>;
	using TextureRendererMapTypeIt = std::unordered_map<EntityID, TextureRenderer*>::iterator;

	using AnimationRendererMapType = std::unordered_map<EntityID, AnimationRenderer*>;
	using AnimationRendererMapTypeIt = std::unordered_map<EntityID, AnimationRenderer*>::iterator;
	*/

	// Functions
	CManager();

	void Init() override;

	void AddComponentCreator(std::string name, IComponentCreator* creator);
	IComponentCreator* GetComponentCreator(std::string name);

	template <typename T>
	T& GetComponent(EntityID id);
	template <typename T>
	typename CMap<T>* GetComponentArray();

	template <typename T>
	void AddComponent(EntityID id, T* component);
	template <typename T>
	void RemoveComponent(EntityID id);

	virtual ~CManager();

private:
	
	// Component creator container
	ComponentMapType component_creator_arr_;

	/*
	// Component containers
	NameMapType name_arr_;
	AABBMapType aabb_arr_;
	ScaleMapType scale_arr_;
	StatusMapType status_arr_;
	HealthMapType health_arr_;
	MotionMapType motion_arr_;
	BasicAIMapType basicai_arr_;
	CliclableMapType clickable_arr_;
	TransformMapType transform_arr_;
	PointLightMapType pointlight_arr_;
	TextRendererMapType text_renderer_arr_;
	InputControllerMapType input_controller_arr_;
	TextureRendererMapType texture_renderer_arr_;
	AnimationRendererMapType animation_renderer_arr_;
	*/

	//// Original implementation thoughts
	//ComponentMap my_component_map_;
};


// When creating a new component, add it to this "List"
using ComponentManager = CManager<
	Name, AI, AABB, Scale, Status, Health, Motion, BasicAI, Clickable, Transform, PointLight, 
	TextRenderer, InputController, TextureRenderer, AnimationRenderer
>;

template <typename T>
typename CMap<T>::MapTypeIt CMap<T>::begin() {

	return component_map_.begin();
}

template <typename T>
typename CMap<T>::MapTypeIt CMap<T>::end() {

	return component_map_.end();
}

template <typename T>
void CMap<T>::AddComponent(EntityID id, T* component) {

	DEBUG_ASSERT(component_map_.find(id) == component_map_.end(), "Component already exists for entity!");
	component_map_.emplace(id, component);
}

template <typename T>
void CMap<T>::RemoveComponent(EntityID id) {

	//DEBUG_ASSERT(component_map_.find(id) != component_map_.end(), "Component does not exist for entity!");

	if (component_map_.find(id) != component_map_.end()) {
		
		component_map_.erase(id);
	}
	else {

		M_DEBUG->WriteDebugMessage("Component belonging to entity w/ id " + std::to_string(id) + " does not exist in map\n");
	}
}

template <typename T>
T* CMap<T>::GetComponent(EntityID id) {

	//DEBUG_ASSERT(component_map_.find(id) == component_map_.end(), "Component does not exist for entity!");
	return (component_map_.find(id) == component_map_.end()) ? nullptr : component_map_[id];
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
	DEBUG_ASSERT((component_creator_arr_.find(name) == component_creator_arr_.end()), "Component creator already exists");

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
typename CMap<T>* CManager<Ts...>::GetComponentArray() {
	return this;
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

#endif