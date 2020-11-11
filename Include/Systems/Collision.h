#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <string>
#include "Systems/Message.h"
#include "Systems/Partitioning.h"
#include "ISystem.h"
#include "Components/AABB.h"
#include "Components/Clickable.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Components/InputController.h"
#include "Manager/ShaderManager.h"
#include "Manager/ModelManager.h"
#include "Manager/ComponentManager.h"
#include <unordered_map>
#include <bitset>
#include <string>

enum class CollisionLayer
{
	BACKGROUND = 0, // Non-interactable
	TILES,
	ENEMY,
	PLAYER,
	HOLE,
	UI_ELEMENTS     // Non-interactable
};

class Collision : public ISystem {

public:

	using AABBType = std::unordered_map<EntityID, AABB*>;
	using AABBIt = AABBType::iterator;

	using ClickableType = CMap<Clickable>;
	using ClickableIt = ClickableType::MapTypeIt;

	using MotionType = CMap<Motion>;
	using MotionIt = MotionType::MapTypeIt;

	using StatusMapType = CMap<Status>;
	using StatusIt = StatusMapType::MapTypeIt;

	using TransformType = CMap<Transform>;
	using TransformIt = TransformType::MapTypeIt;

	using ScaleType = CMap<Scale>;
	using ScaleIt = ScaleType::MapTypeIt;

	using InputControllerType = CMap<InputController>;
	using InputControllerIt = InputControllerType::MapTypeIt;

	// Placeholder stuff, testing Collision Layering
	typedef std::pair<std::bitset<10>, bool> CollidableLayers;

	using CollidableLayer = std::bitset<10>;

	using CollisionLayerIt = std::unordered_map<CollisionLayer, CollidableLayers>::iterator;

	using CollisionMapType = std::map<CollisionLayer, AABBType>;

	using CollisionMapIt = CollisionMapType::iterator;
	using CollisionMapReverseIt = CollisionMapType::reverse_iterator;
private:
	//For debug drawing
	bool debug_;
	Model* model_;
	Shader* shdr_pgm_;
	glm::mat3* world_to_ndc_xform_;
	float* cam_zoom_;

	AABBType aabb_arr_;

	ClickableType* clickable_arr_;

	MotionType* motion_arr_;

	StatusMapType* status_arr_;

	TransformType* transform_arr_;

	ScaleType* scale_arr_;

	InputControllerType* input_controller_arr_;

	std::unordered_map<CollisionLayer, CollidableLayers> collision_layer_arr_;

	std::map<CollisionLayer, AABBType> collision_map_;

	PartitioningSystem* partitioning_;
	EntityManager* entity_mgr_;
	ComponentManager* component_mgr_;

/******************************************************************************/
/*!
  \fn AddCollisionLayers()

  \brief Helper function to set up collision layers for collision layer map
*/
/******************************************************************************/
	void AddCollisionLayers(CollisionLayer layer, const std::string& collidables, bool collide_self = true);

/******************************************************************************/
/*!
  \fn SeparatingAxisTheorem()

  \brief Returns true if there is at least 1 intersection axis
*/
/******************************************************************************/
	bool SeparatingAxisTheorem(const AABB& a, const AABB& b);

/******************************************************************************/
/*!
  \fn DefaultResponse()

  \brief Helper function to handle response of a dynamic entity colliding
		 with a wall
*/
/******************************************************************************/
	void DefaultResponse(AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2, float frametime, float t_first);

/******************************************************************************/
/*!
  \fn PlayervEnemyResponse()

  \brief Helper function to handle response of a player colliding with an enemy
*/
/******************************************************************************/
	bool PlayervEnemyResponse(AABBIt aabb1, AABBIt aabb2);

/******************************************************************************/
/*!
  \fn CollisionResponse()

  \brief Main function that handles collision response between 2 entities
*/
/******************************************************************************/
	void CollisionResponse(const CollisionLayer& col_layer_a, const CollisionLayer& col_layer_b, 
						   AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2, 
						   float frametime, float t_first);

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
  \fn CheckCursorCollision()

  \brief Checks for collision between mouse cursor and a menu entity
*/
/******************************************************************************/
	bool CheckCursorCollision(const Vec2& cursor_pos, const AABB* box);

public:

/******************************************************************************/
/*!
  \fn Collision()

  \brief Constructor for Collision, initialises data members
*/
/******************************************************************************/
	Collision();

/******************************************************************************/
/*!
  \fn CheckCollision()

  \brief Checks for static and dynamic collision between two entities,
         returns true if collision happens between the objects within
		 the current frame
*/
/******************************************************************************/
	bool CheckCollision(const AABB &aabb1, const Vec2 &vel1, 
						const AABB &aabb2, const Vec2 &vel2,
						const float dt, float& tFirst);
	
/******************************************************************************/
/*!
  \fn CheckClickableCollision()

  \brief Checks for collision between mouse cursor and a menu entity
*/
/******************************************************************************/
	void CheckClickableCollision();
	
/******************************************************************************/
/*!
  \fn BurrowReady()

  \brief Helper function to check if a player is over a hole before burrowing
*/
/******************************************************************************/
	bool BurrowReady();

/******************************************************************************/
/*!
  \fn SelectEntity()

  \brief Returns the EntityID of an object that has been selected by the cursor
*/
/******************************************************************************/
	EntityID SelectEntity();

/******************************************************************************/
/*!
  \fn GetAttachedComponentIDs()

  \brief Retrieves a pair of Entity* and vector of ComponentTypes when cursor 
		 selects an entity 
*/
/******************************************************************************/
	std::pair<Entity*, std::vector<ComponentTypes>> GetAttachedComponentIDs();

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
  \fn ProcessCollision()

  \brief Helper function to handle collision checking between 2 layers
*/
/******************************************************************************/
	void ProcessCollision(CollisionMapIt col_layer_a, CollisionMapIt col_layer_b, float frametime);

/******************************************************************************/
/*!
  \fn GetPartitionedCollisionMap()

  \brief Converts paritioned entities from vector into a collision layered map
*/
/******************************************************************************/
	void GetPartitionedCollisionMap(size_t x, size_t y, CollisionMapType& col_map);

/******************************************************************************/
/*!
  \fn ProcessParitionedEntities()

  \brief Helper function to handle collision checking between entities in a
		 partition
*/
/******************************************************************************/
	void ProcessPartitionedEntities(size_t y, size_t x, float frametime);

/******************************************************************************/
/*!
  \fn ~Collision()

  \brief Default implementation destructor
*/
/******************************************************************************/
	virtual ~Collision() = default;
};

#endif