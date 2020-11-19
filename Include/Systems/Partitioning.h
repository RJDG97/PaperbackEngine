#pragma once
#ifndef _PARTITIONING_H_
#define _PARTITIONING_H_

#include <vector>
#include <bitset>
#include "Systems/ISystem.h"
#include "Manager/ComponentManager.h"
#include "MathLib/MathHelper.h"


class PartitioningSystem : public ISystem
{
public:

	using Bitset = std::bitset<1700>;
	using PartitionAxis = std::vector< Bitset >;

	using TransformMap = CMap<Transform>;
	using TransformMapIt = TransformMap::MapTypeIt;

	using AABBMap = CMap<AABB>;
	using AABBMapIt = AABBMap::MapTypeIt;

/******************************************************************************/
/*!
  \fn Init()

  \brief Used to initialise a system if required;
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn InitPartition()

  \brief Used to init the partition after a map has been loaded
*/
/******************************************************************************/
	void InitPartition();

/******************************************************************************/
/*!
  \fn Update()

  \brief Used to run logic that is required every game loop.
*/
/******************************************************************************/
	void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	std::string GetName() override { return "Partitioning"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Handles incoming messages and sorts based on message id
*/
/******************************************************************************/
	void SendMessageD(Message* m) override;

/******************************************************************************/
/*!
  \fn GetCoordBitset()

  \brief Returns a copy of the bitset at a specified location
*/
/******************************************************************************/
	Bitset GetCoordBitset(size_t x, size_t y) const;

/******************************************************************************/
/*!
  \fn ResetPartition()

  \brief Resets all patitions
*/
/******************************************************************************/
	void ResetPartition();

/******************************************************************************/
/*!
  \fn GetPartitionedEntities()

  \brief Stores all AABB iterators within the [x, y] partition of size "grid_size_"
*/
/******************************************************************************/
	void GetPartitionedEntities(std::vector<AABBMapIt>& vec, size_t x, size_t y);
	
/******************************************************************************/
/*!
  \fn VerifyPartition()

  \brief Ensures that partition has at least 2 entities for collision
*/
/******************************************************************************/
	bool VerifyPartition(size_t x, size_t y);

/******************************************************************************/
/*!
  \fn GetAxisSizes()

  \brief Gets the sizes of both axis
*/
/******************************************************************************/
	std::pair<size_t, size_t> GetAxisSizes();

/******************************************************************************/
/*!
  \fn ConvertTransformToGridScale()

  \brief Converts world coordinates to grid coordinates
*/
/******************************************************************************/
	Vector2D ConvertTransformToGridScale(const Vector2D& pos);

private:

	ComponentManager* component_manager_;
	TransformMap* transform_map_;
	AABBMap* aabb_map_;

	PartitionAxis x_, y_;
	size_t grid_size_;
	Vector2D abs_bottom_left_;
	Vector2D abs_top_right_;
};


#endif