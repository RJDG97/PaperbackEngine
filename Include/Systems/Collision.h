#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <string>
#include "Systems/Message.h"
#include "ISystem.h"
#include "Components/AABB.h"
#include "Components/Clickable.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include "Components/Transform.h"
#include "Components/InputController.h"
#include "Manager/ShaderManager.h"
#include "Manager/ModelManager.h"
#include <unordered_map>

class Collision : public ISystem {

	//For debug drawing
	bool debug_;
	Model model_;
	Shader shdr_pgm_;
	glm::mat3* world_to_ndc_xform_;

	using AABBIt = std::unordered_map<EntityID, AABB*>::iterator;
	std::unordered_map<EntityID, AABB*> aabb_arr_;

	using ClickableIt = std::unordered_map<EntityID, Clickable*>::iterator;
	std::unordered_map<EntityID, Clickable*> clickable_arr_;

	using MotionIt = std::unordered_map<EntityID, Motion*>::iterator;
	std::unordered_map<EntityID, Motion*> motion_arr_;

	using StatusIt = std::unordered_map<EntityID, Status*>::iterator;
	std::unordered_map<EntityID, Status*> status_arr_;
	
	using TransformIt = std::unordered_map<EntityID, Transform*>::iterator;
	std::unordered_map<EntityID, Transform*> transform_arr_;

	using InputControllerIt = std::unordered_map<EntityID, InputController*>::iterator;
	std::unordered_map < EntityID, InputController*> input_controller_arr_;

/******************************************************************************/
/*!
  \fn UpdateBoundingBox()

  \brief Helper function to recompute a component's current bounding box
		 coordinates, using the Scale component for size of the box
*/
/******************************************************************************/
	void UpdateBoundingBox();

/******************************************************************************/
/*!
  \fn UpdateClickableBB()

  \brief Helper function to recompute a component's current bounding box
		 coordinates, using the Scale component for size of the box
*/
/******************************************************************************/
	void UpdateClickableBB();

/******************************************************************************/
/*!
  \fn CheckCursorCollision()

  \brief Checks for collision between mouse cursor and a menu entity
*/
/******************************************************************************/
	bool CheckCursorCollision(const Vec2& cursor_pos, const Clickable* button);

/******************************************************************************/
/*!
  \fn SeparatingAxisTheorem()

  \brief Returns true if there is at least 1 intersection axis
*/
/******************************************************************************/
	bool SeparatingAxisTheorem(const AABB& a, const AABB& b);

/******************************************************************************/
/*!
  \fn CollisionWall()

  \brief Helper function to handle response of a non-wall entity colliding
		 with a wall
*/
/******************************************************************************/
	void CollisionWall(AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2, float frametime, float t_first);
public:

/******************************************************************************/
/*!
  \fn Collision()

  \brief Constructor for Collision, initialises data members
*/
/******************************************************************************/
	Collision();

	bool CheckCollision(const AABB &aabb1, const Vec2 &vel1, 
						const AABB &aabb2, const Vec2 &vel2,
						const float dt, float& tFirst);
	
/******************************************************************************/
/*!
  \fn CheckCursorCollision()

  \brief Checks for collision between mouse cursor and a menu entity
*/
/******************************************************************************/
	void CheckClickableCollision();

/******************************************************************************/
/*!
  \fn AddAABBComponent()

  \brief Adds a AABB component to the aabb map
*/
/******************************************************************************/
	void AddAABBComponent(EntityID id, AABB* aabb);

/******************************************************************************/
/*!
  \fn RemoveAABBComponent()

  \brief Removes a AABB component from the aabb map
*/
/******************************************************************************/
	void RemoveAABBComponent(EntityID id);

/******************************************************************************/
/*!
  \fn AddClickableComponent()

  \brief Adds a Clickable component to the clickable map
*/
/******************************************************************************/
	void AddClickableComponent(EntityID id, Clickable* clickable);

/******************************************************************************/
/*!
  \fn RemoveClickableComponent()

  \brief Removes a Clickable component from the clickable map
*/
/******************************************************************************/
	void RemoveClickableComponent(EntityID id);

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
  \fn AddTransformComponent()

  \brief Adds a newly created Transform Component to the status component array
		 within the Collision System
*/
/******************************************************************************/
	void AddTransformComponent(EntityID id, Transform* status);

/******************************************************************************/
/*!
  \fn RemoveTransformComponent()

  \brief Removes a Transform Component from the status component array within the
		 Collision System
*/
/******************************************************************************/
	void RemoveTransformComponent(EntityID id);

/******************************************************************************/
/*!
  \fn AddInputControllerComponent()

  \brief Adds a newly created InputController Component to the InputController 
		 component array within the Game System
*/
/******************************************************************************/
	void AddInputControllerComponent(EntityID id, InputController* input_controller);

/******************************************************************************/
/*!
  \fn RemoveInputControllerComponent()

  \brief Removes a InputController Component from the InputController 
		 component array within the Game System
*/
/******************************************************************************/
	void RemoveInputControllerComponent(EntityID id);

/******************************************************************************/
/*!
  \fn Init()

  \brief Initializes the lines required for debug drawing of bounding boxes
*/
/******************************************************************************/
	virtual void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Performs update of AABB as well as handle checks for collision as well
		 as the follow up actions for colliding events
*/
/******************************************************************************/
	virtual void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn Draw()

  \brief Used to draw the bounding boxes of entities if debug mode is enabled
*/
/******************************************************************************/
	virtual void Draw() override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	virtual std::string GetName() override { return "Collision"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	virtual void SendMessageD(Message* m) override;

/******************************************************************************/
/*!
  \fn ~Collision()

  \brief Default implementation destructor
*/
/******************************************************************************/
	virtual ~Collision() = default;
};

#endif