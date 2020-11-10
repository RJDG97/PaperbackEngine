#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "Systems/ISystem.h"
#include "Entity/Entity.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Systems/Message.h"
#include "Components/Transform.h"
#include "Components/Health.h"
#include "Systems/Factory.h"
#include "Systems/GraphicsSystem.h"
#include "Manager/ForcesManager.h"
#include "Manager/ComponentManager.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include <unordered_map>

class Physics : public ISystem
{
public:

/******************************************************************************/
/*!
  \fn ChangeVelocity()

  \brief Used to change the velocity of all components that have the Player
		 entity type
*/
/******************************************************************************/
	void ChangeVelocity(Message* m);

/******************************************************************************/
/*!
  \fn AddTransformComponent()

  \brief Adds a Transform component to the transform map
*/
/******************************************************************************/
	void AddTransformComponent(EntityID id, Transform* transform);

/******************************************************************************/
/*!
  \fn RemoveTransformComponent()

  \brief Removes a Transform component from the transform map
*/
/******************************************************************************/
	void RemoveTransformComponent(EntityID id);

/******************************************************************************/
/*!
  \fn AddMotionComponent()

  \brief Adds a Motion component to the motion map
*/
/******************************************************************************/
	void AddMotionComponent(EntityID id, Motion* motion);

/******************************************************************************/
/*!
  \fn RemoveMotionComponent()

  \brief Removes a Motion component from the motion map
*/
/******************************************************************************/
	void RemoveMotionComponent(EntityID id);

/******************************************************************************/
/*!
  \fn AddStatusComponent()

  \brief Adds a Status component to the status map
*/
/******************************************************************************/
	void AddStatusComponent(EntityID id, Status* status);

/******************************************************************************/
/*!
  \fn RemoveStatusComponent()

  \brief Removes a Status component from the status map
*/
/******************************************************************************/
	void RemoveStatusComponent(EntityID id);

/******************************************************************************/
/*!
  \fn Physics()

  \brief Constructor for the Physics System
*/
/******************************************************************************/
	Physics();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialises the system
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Computes the new velocity for every Motion component and then
		 updates the Transform components based on that new Motion component
*/
/******************************************************************************/
	virtual void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() override { return "Physics"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	void SendMessageD(Message* msg) override;

private:

	bool debug_;
	ForcesManager* force_mgr;
	GraphicsSystem* graphics_sys_;
	ComponentManager* component_mgr_;

	//using TransformIt = std::unordered_map<EntityID, Transform*>::iterator;
	//std::unordered_map<EntityID,Transform*> transform_arr_;
	using TransformType = CMap<Transform>;
	using TransformIt = TransformType::MapTypeIt;
	TransformType* transform_arr_;

	//using MotionIt = std::unordered_map<EntityID, Motion*>::iterator;
	//std::unordered_map<EntityID, Motion*> motion_arr_;
	using MotionType = CMap<Motion>;
	using MotionIt = MotionType::MapTypeIt;
	MotionType* motion_arr_;

	//using StatusIt = std::unordered_map<EntityID, Status*>::iterator;
	//std::unordered_map<EntityID, Status*> status_arr_;
	using StatusMapType = CMap<Status>;
	using StatusIt = StatusMapType::MapTypeIt;
	StatusMapType* status_arr_;
};

#endif