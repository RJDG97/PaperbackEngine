#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <string>
#include "Systems/Message.h"
#include "ISystem.h"
#include "Components/AABB.h"
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

/******************************************************************************/
/*!
  \fn UpdateBoundingBox()

  \brief Helper function to recompute a component's current bounding box
		 coordinates, using the Scale component for size of the box
*/
/******************************************************************************/
	void UpdateBoundingBox();

	//handles the response from colliding with a wall
/******************************************************************************/
/*!
  \fn UpdateBoundingBox()

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
	bool CheckCursorCollision(const Vec2& cursor_pos, const EntityID& button_id);

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