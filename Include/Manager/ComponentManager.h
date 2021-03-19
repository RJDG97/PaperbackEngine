/**********************************************************************************
*\file         ComponentManager.h
*\brief        Contains definition and declaration of functions and variables used for
*			   the Component Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
#include "Components/ConeLight.h"
#include "Components/AABB.h"
#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/BasicAI.h"
#include "Components/TextureRenderer.h"
#include "Components/AnimationRenderer.h"
#include "Components/TextRenderer.h"
#include "Components/Camera.h"
#include "Components/Clickable.h"
#include "Components/InputController.h"
#include "Components/AI.h"
#include "Components/ParentChild.h"
#include "Components/LogicComponent.h"
#include "Components/Inventory.h"
#include "Components/Particle.h"
#include "Components/Emitter.h"
#include "Components/SoundEmitter.h"
#include "Components/Collectible.h"
#include "Components/Unlockable.h"
#include "Components/Interactable.h"
#include "Components/Dialogue.h"
#include "Components/Child.h"
#include "Components/Puzzle.h"
#include "Components/StopAnimation.h"
#include "Components/Destination.h"
#include "Components/UIParticle.h"

template <typename T>
class CMap
{
public:
	using MapType = std::unordered_map<EntityID, T*>;
	using MapTypeIt = typename MapType::iterator;

/******************************************************************************/
/*!
  \fn begin()

  \brief Returns an iterator to the beginning of the Type component map
*/
/******************************************************************************/
	MapTypeIt begin();

/******************************************************************************/
/*!
  \fn end()

  \brief Returns an iterator to the end of the Type component map
*/
/******************************************************************************/
	MapTypeIt end();

/******************************************************************************/
/*!
	\fn size()

	\brief Returns size of the Type component map
*/
/******************************************************************************/
	size_t size();

/******************************************************************************/
/*!
  \fn AddComponent()

  \brief Adds a component to the Type component map
*/
/******************************************************************************/
	void AddComponent(EntityID id, T* component);

/******************************************************************************/
/*!
  \fn RemoveComponent()

  \brief Remove a component from the Type component map
*/
/******************************************************************************/
	void RemoveComponent(EntityID id);

/******************************************************************************/
/*!
  \fn GetComponent()

  \brief Returns an component of Type attached to the entity
*/
/******************************************************************************/
	T* GetComponent(EntityID id);

/******************************************************************************/
/*!
  \fn GetComponentIt()

  \brief Get iterator to a component attached to the entity
*/
/******************************************************************************/
	MapTypeIt GetComponentIt(EntityID id);

/******************************************************************************/
/*!
  \fn ~CMap()

  \brief CMap destructor
*/
/******************************************************************************/
	~CMap();
private:

	MapType component_map_;
};




template <typename... Ts>
class CManager : public IManager, private CMap<Ts>...
{
public:

	// Component Creator
	using ComponentMapType = std::unordered_map<std::string, IComponentCreator*>;
	using ComponentMapTypeIt = std::unordered_map<std::string, IComponentCreator*>::iterator;



/******************************************************************************/
/*!
  \fn CManager()

  \brief Constructor for CManager
*/
/******************************************************************************/
	CManager();

/******************************************************************************/
/*!
  \fn Init()

  \brief Init for CManager
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn AddComponentCreator()

  \brief Returns an iterator to the end of the Type component map
*/
/******************************************************************************/
	void AddComponentCreator(std::string name, IComponentCreator* creator);

/******************************************************************************/
/*!
  \fn GetComponentCreator()

  \brief Returns a IComponentCreator for a component
*/
/******************************************************************************/
	IComponentCreator* GetComponentCreator(std::string name);

/******************************************************************************/
/*!
  \fn GetComponentList()

  \brief Retrieves a map of IComponentCreators
*/
/******************************************************************************/
	ComponentMapType& GetComponentList();


/******************************************************************************/
/*!
  \fn GetComponent()

  \brief Returns a Component* from the matching entity
*/
/******************************************************************************/
	template <typename T>
	T* GetComponent(EntityID id);

/******************************************************************************/
/*!
  \fn GetComponentArray()

  \brief Returns a CMap of type T
*/
/******************************************************************************/
	template <typename T>
	typename CMap<T>* GetComponentArray();

/******************************************************************************/
/*!
  \fn AddComponent()

  \brief Add a Component* to the coresponding CMap
*/
/******************************************************************************/
	template <typename T>
	void AddComponent(EntityID id, T* component);

/******************************************************************************/
/*!
  \fn RemoveComponent()

  \brief Remove a Component* from the coresponding CMap
*/
/******************************************************************************/
	template <typename T>
	void RemoveComponent(EntityID id);

/******************************************************************************/
/*!
  \fn ~CManager()

  \brief Destructor for CManager
*/
/******************************************************************************/
	virtual ~CManager();

private:
	
	// Component creator container
	ComponentMapType component_creator_arr_;
};



// When creating a new component, add it to this "List"
using ComponentManager = CManager<
	Name, AI, AABB, Scale, Status, Health, Motion, BasicAI, Clickable, Transform, PointLight, ConeLight, Camera,
	TextRenderer, InputController, TextureRenderer, AnimationRenderer, ParentChild, LogicComponent, Inventory,
	Emitter, Particle, SoundEmitter, Collectible, Unlockable, Interactable, DialogueTrigger, Child, Puzzle,
	StopAnimation, Destination, UIParticle
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
size_t CMap<T>::size() {

	return component_map_.size();
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
typename CMap<T>::MapTypeIt CMap<T>::GetComponentIt(EntityID id) {

	return (component_map_.find(id) == component_map_.end()) ? component_map_.end() : component_map_.find(id);
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
typename CManager<Ts...>::ComponentMapType& CManager<Ts...>::GetComponentList()
{
	return component_creator_arr_;
}


template <typename... Ts>
template <typename T>
T* CManager<Ts...>::GetComponent(EntityID id) {
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