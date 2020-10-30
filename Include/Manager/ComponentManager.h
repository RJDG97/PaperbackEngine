#pragma once
#ifndef _COMPONENT_MANAGER_H_
#define _COMPONENT_MANAGER_H_

#include <map>
#include <string>
#include <unordered_map>

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

template <typename T>
class CMap
{
public:
	void AddComponent(EntityID id, T* component);
	void RemoveComponent(EntityID id);
	T& GetComponent(EntityID id);
	~CMap();
private:
	using MapType = std::unordered_map<EntityID, T*>;
	using MapTypeIt = typename MapType::iterator;

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
	T& GetComponentArray();

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
	Name, AABB, Scale, Status, Health, Motion, BasicAI, Clickable, Transform, PointLight, 
	TextRenderer, InputController, TextureRenderer, AnimationRenderer
>;

#endif