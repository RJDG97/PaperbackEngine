/**********************************************************************************
*\file         AMap.cpp
*\brief        Contains definition of functions and variables used for
*			   the AMap System
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/AMap.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
#include "Systems/Collision.h"
#include "Components/AABB.h"
#include "Engine/Core.h"

void AMap::Init() {
	
}

// To be called after a game state initializes it's entities
void AMap::InitAMap(std::map<EntityID, Entity*> entity_map) {

	AMapInitialization(entity_map);

	ComponentManager* component_manager_ = &*CORE->GetManager<ComponentManager>();
	Vector2D pos{};

	for (EntityManager::EntityIdMapTypeIt it = entity_map.begin(); it != entity_map.end(); ++it) {

		Transform* transform = component_manager_->GetComponent<Transform>(it->first);

		if (!transform)
			continue;

		Vector2D abs_min;
		abs_min.x = bottom_left_.x < 0 ? -bottom_left_.x : bottom_left_.x;
		abs_min.y = bottom_left_.y < 0 ? -bottom_left_.y : bottom_left_.y;

		pos = transform->GetOffsetAABBPos();
		pos += abs_min;

		AABB* aabb = component_manager_->GetComponent<AABB>(it->first);
		if (aabb && (aabb->GetLayer() == static_cast<size_t>(CollisionLayer::TILES)))
			InsertEntityNodes(pos, aabb->GetAABBScale());
	}
}

// For use during run-time if there are necessary updates
void AMap::UpdateAMap(std::pair<EntityID, Entity*> entity) {

	(void)entity;
}

Vector2D AMap::GetBottomLeft() {

	return bottom_left_;
}

Vector2D AMap::GetTopRight() {

	return top_right_;
}

// Helper function to determine the "boundaries" of the level
void AMap::AMapInitialization(std::map<EntityID, Entity*> entity_map) {

	ComponentManager* component_manager_ = &*CORE->GetManager<ComponentManager>();
	Vector2D position{};
	Vector2D size{};

	top_right_ = {};
	bottom_left_ = {};

	// Iterates through all entities to determine the bottom left and top right coords
	for (EntityManager::EntityIdMapTypeIt it = entity_map.begin(); it != entity_map.end(); ++it) {

		Transform* transform = component_manager_->GetComponent<Transform>(it->first);

		if (!transform)
			continue;

		position = transform->GetOffsetAABBPos();


		top_right_.x = position.x > top_right_.x ? position.x : top_right_.x;
		top_right_.y = position.y > top_right_.y ? position.y : top_right_.y;
		bottom_left_.x = position.x < bottom_left_.x ? position.x : bottom_left_.x;
		bottom_left_.y = position.y < bottom_left_.y ? position.y : bottom_left_.y;
	}

	// Compute "distance" between both sides
	size = top_right_ - bottom_left_;

	// Resize 2D vector of Nodes and initialize all Node neighbours
	SetAMapSize(size);
	SetNodeNeighbours();
}

void AMap::ClearMap() {

	for (int i = 0; i < node_map_.size(); ++i) {
		node_map_.clear();
	}
	node_map_.clear();
}

// Resizes the 2D vector of Nodes
void AMap::SetAMapSize(Vector2D& size) {

	ClearMap();

	node_map_.resize(static_cast<size_t>(size.y));

	for (AMapTypeItY it = node_map_.begin(); it != node_map_.end(); ++it) {

		it->resize(static_cast<size_t>(size.x));
	}
}

// Initialize all Node neighbours
void AMap::SetNodeNeighbours() {
	for (int i = 0; i < node_map_.size(); ++i) {
		for (int j = 0; j < node_map_[0].size(); ++j) {
		
			node_map_[i][j].obstacle_ = false;
			node_map_[i][j].visited_ = false;
			node_map_[i][j].nodepos_ = {static_cast<float>(j), static_cast<float>(i)};
			
			for (int y = -1; y < 2; ++y) {
				for (int x = -1; x < 2; ++x) {

					//if corners are hit then skip
					//if ((y == -1 && (x == -1 || x == 1) || (y == 1 && (x == -1 || x == 1))))
					//	continue;

					int height = i + y;
					int width = j + x;

					if ((height >= 0) && height < node_map_.size() &&
						(width >= 0) && width < node_map_[0].size()) {
						if (node_map_[height][width].nodepos_.x == node_map_[i][j].nodepos_.x &&
							node_map_[height][width].nodepos_.y == node_map_[i][j].nodepos_.y)
							continue;
						node_map_[i][j].neighbour_.push_back( &node_map_[height][width] );
					}
				}
			}
		}
	}
}

// Initialize all entities positions to be occupied
void AMap::InsertEntityNodes(const Vector2D& pos, const Vector2D& scale) {

	if (pos.y >= node_map_.size() || pos.y < 0)
		return;
	if (pos.x >= node_map_[0].size() || pos.x < 0)
		return;
	
	float min_x, min_y, max_x, max_y;

	// Computing the min and max pos of the entity
	min_x = (pos.x - scale.x);
	RoundUp(min_x);
	min_y = (pos.y - scale.y);
	RoundUp(min_y);
	max_x = (pos.x + scale.x);
	RoundUp(max_x);
	max_y = (pos.y + scale.y);
	RoundUp(max_y);

	// Setting the bits for the grid based on entity location
	for (size_t j = static_cast<size_t>(min_x); j < max_x && j >= 0 && j < node_map_[0].size(); ++j) {

		for (size_t i = static_cast<size_t>(min_y); i < max_y && i >= 0 && i < node_map_.size(); ++i) {
		
			node_map_[i][j].obstacle_ = true;

			//for(auto n : node_map_[i][j].neighbour_)
			//	node_map_[i][j].obstacle_ = true;
		}
	}
}

AMap::AMapTypeY AMap::GetNodeMap()
{
	return node_map_;
}

void AMap::InitializeNodes()
{
	for (int i = static_cast<int>(node_map_.size() - 1); i >= 0; --i) {
		for (int j = 0; j < static_cast<int>(node_map_[0].size()); ++j) {
			node_map_[i][j].start_ = false;
			node_map_[i][j].des_ = false;
			node_map_[i][j].visited_ = false;
			node_map_[i][j].path_ = false;
			node_map_[i][j].parent_ = nullptr;
			node_map_[i][j].F = 0;
			node_map_[i][j].G = 0;
			node_map_[i][j].H = 0;
		}
	}
}

bool AMap::inlist(std::list<node> L, node N)
{
	for (std::list<node>::iterator it = L.begin();
		it != L.end(); ++it)
	{
		if (it->nodepos_.x == N.nodepos_.x && it->nodepos_.y == N.nodepos_.y)
			return true;
	}
	return false;
}

bool AMap::Pathing(std::vector<Vector2D>&  path,Vector2D start, Vector2D des)
{
	// Reset nodes
	InitializeNodes();

	path.clear();

	// Localize coordinates
	Vector2D abs_min;
	abs_min.x = bottom_left_.x < 0 ? -bottom_left_.x : bottom_left_.x;
	abs_min.y = bottom_left_.y < 0 ? -bottom_left_.y : bottom_left_.y;

	// Set start and destination nodes
	node* startnode = &node_map_[static_cast<size_t>((start + abs_min).y)][static_cast<size_t>((start + abs_min).x)];
	node* desnode = &node_map_[static_cast<size_t>((des + abs_min).y)][static_cast<size_t>((des + abs_min).x)];
	node* currentnode = nullptr;

	// Set for Drawing on map
	startnode->start_ = true;
	desnode->des_ = true;

	if (desnode->obstacle_)
		return false;

	std::list<node> openlist;
	std::list<node> closedlist;
	// Push starting node into front of the list
	openlist.push_front(*startnode);

	while (!openlist.empty())
	{
		// Set current node as node with lowest F or H cost
		openlist.sort([](const node lhs, const node rhs) { return (lhs.F < rhs.F || lhs.F == rhs.F && lhs.H < rhs.H); });
		
		// Set current node to start of the list
		currentnode = &node_map_[static_cast<size_t>(openlist.front().nodepos_.y)][static_cast<size_t>(openlist.front().nodepos_.x)];

		// Remove node from openlist and add to closed list
		closedlist.push_front(openlist.front());
		openlist.pop_front();

		// If des reached exit
		if (currentnode == desnode)
		{
			while (currentnode->parent_)
			{
				currentnode->path_ = true;
				path.push_back(currentnode->nodepos_ - abs_min);
				currentnode = currentnode->parent_;
			}
			return true;
		}

		for (auto nnode : currentnode->neighbour_)
		{

			if (nnode->obstacle_ || inlist(closedlist, *nnode))
				continue;

			int newNeighbourCost = static_cast<int>(currentnode->G +
				Vector2DDistance(currentnode->nodepos_, nnode->nodepos_));
			if (newNeighbourCost < nnode->G || !inlist(openlist, *nnode)) {
				nnode->G = static_cast<float>(newNeighbourCost);
				nnode->H = Vector2DDistance(nnode->nodepos_, desnode->nodepos_);
				nnode->F = nnode->G + nnode->H;
				nnode->parent_ = currentnode;
				nnode->visited_ = true;

				// Insert node into open list if not in list
				if (!inlist(openlist, *nnode))
					openlist.push_front(*nnode);
			}
		}
	}
	return false;
}

void AMap::DrawMap()
{
	std::cout << "---------------------------" << std::endl;
	for (int i = static_cast<int>(node_map_.size()-1); i >= 0; --i) {
		for (int j = 0; j < static_cast<int>(node_map_[0].size()); ++j) {
			std::cout << "|";
			if(node_map_[i][j].start_)
				std::cout << "S";
			else if (node_map_[i][j].des_)
				std::cout << "D";
			else if (node_map_[i][j].path_)
				std::cout << "*";
			else if (node_map_[i][j].visited_)
				std::cout << ".";
			else if (node_map_[i][j].obstacle_)
				std::cout << "X";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
}