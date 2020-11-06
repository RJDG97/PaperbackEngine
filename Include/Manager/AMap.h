#pragma once
#ifndef _A_MAP_H_
#define _A_MAP_H_

#include <map>
#include <vector>
#include "MathLib/Vector2D.h"
#include "Entity/Entity.h"
#include "Manager/IManager.h"
#include "Systems/Pathfinding.h"

class AMap : public IManager
{
public:

	using AMapTypeX = std::vector<Pathfinding::Node>;
	using AMapTypeY = std::vector<AMapTypeX>;
	using AMapTypeItX = AMapTypeX::iterator;
	using AMapTypeItY = AMapTypeY::iterator;
	
	void Init() override;
	void InitAMap(std::map<EntityID, Entity*> entity_map);
	void UpdateAMap(std::pair<EntityID, Entity*> entity);
	

private:
	
	Vector2D top_right_;
	Vector2D bottom_left_;
	AMapTypeY node_map_;


	void AMapInitialization(std::map<EntityID, Entity*> entity_map);
	void SetAMapSize(Vector2D& size);
	void SetNodeNeighbours();
	void InsertEntityNodes(const Vector2D& pos);
};

#endif