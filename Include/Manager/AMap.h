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
	struct node
	{
		Vector2D nodepos_;
		float F, G, H;

		bool start_;
		bool des_;
		bool obstacle_;
		bool visited_;
		node* parent_;
		std::vector<node*> neighbour_;
	};

	// F variable for least cost from one node to another (for optimal path)
	// G cost from current node to the next
	// H heuristic distant from current node to destination node
	
	// Cost to enter source and enter destination are zero
	
	// F = G + H
	
	// Final path is the lowest cost 

public:

	//using AMapTypeX = std::vector<Pathfinding::Node>;
	using AMapTypeX = std::vector<node>;
	using AMapTypeY = std::vector<AMapTypeX>;
	using AMapTypeItX = AMapTypeX::iterator;
	using AMapTypeItY = AMapTypeY::iterator;
	
	void Init() override;
	void InitAMap(std::map<EntityID, Entity*> entity_map);
	void UpdateAMap(std::pair<EntityID, Entity*> entity);
	AMapTypeY GetNodeMap();
	void DrawMap();
	void UpdatePath(Vector2D start, Vector2D des);
	void Astar(Vector2D start, Vector2D des);

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