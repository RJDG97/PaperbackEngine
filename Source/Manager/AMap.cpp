#include "Manager/AMap.h"
#include "Manager/ComponentManager.h"
#include "Manager/EntityManager.h"
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

		pos = transform->GetPosition();
		pos += abs_min;

		InsertEntityNodes(pos);
	}
}

// For use during run-time if there are necessary updates
void AMap::UpdateAMap(std::pair<EntityID, Entity*> entity) {

	(void)entity;
}

// Helper function to determine the "boundaries" of the level
void AMap::AMapInitialization(std::map<EntityID, Entity*> entity_map) {

	ComponentManager* component_manager_ = &*CORE->GetManager<ComponentManager>();
	Vector2D position{};
	Vector2D size{};

	// Iterates through all entities to determine the bottom left and top right coords
	for (EntityManager::EntityIdMapTypeIt it = entity_map.begin(); it != entity_map.end(); ++it) {

		Transform* transform = component_manager_->GetComponent<Transform>(it->first);

		if (!transform)
			continue;

		position = transform->GetPosition();


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

// Resizes the 2D vector of Nodes
void AMap::SetAMapSize(Vector2D& size) {

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
			node_map_[i][j].parent_ = nullptr;
			node_map_[i][j].nodepos_ = {static_cast<float>(j), static_cast<float>(i)};
			
			for (int y = -1; y < 2; ++y) {
				for (int x = -1; x < 2; ++x) {

					int height = i + y;
					int width = j + x;
				
					if ((height >= 0) && height < node_map_.size() &&
						(width >= 0) && width < node_map_[0].size()) {
						node_map_[i][j].neighbour_.push_back( &node_map_[height][width] );
					}
				}
			}
		}
	}
}

// Initialize all entities positions to be occupied
void AMap::InsertEntityNodes(const Vector2D& pos) {

	if (pos.y >= node_map_.size() || pos.y < 0)
		return;
	if (pos.x >= node_map_[0].size() || pos.x < 0)
		return;

	node_map_[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)].obstacle_ = true;
}

AMap::AMapTypeY AMap::GetNodeMap()
{
	return node_map_;
}

void AMap::Astar(Vector2D start, Vector2D des)
{
	// Reset nodes
	for (int i = node_map_.size() - 1; i >= 0; --i) {
		for (int j = 0; j < node_map_[0].size(); ++j) {
			node_map_[i][j].start_ = false;
			node_map_[i][j].des_ = false;
			node_map_[i][j].visited_ = false;
			node_map_[i][j].parent_ = nullptr;
			node_map_[i][j].F = INFINITY;
			node_map_[i][j].G = INFINITY;
			node_map_[i][j].H = INFINITY;
		}
	}

	// Add start node to the list
	Vector2D abs_min;
	abs_min.x = bottom_left_.x < 0 ? -bottom_left_.x : bottom_left_.x;
	abs_min.y = bottom_left_.y < 0 ? -bottom_left_.y : bottom_left_.y;
		// Set start and destination nodes
	node* startnode = &node_map_[static_cast<int>((start + abs_min).y)][static_cast<int>((start + abs_min).x)];
	node* desnode = &node_map_[static_cast<int>((des + abs_min).y)][static_cast<int>((des + abs_min).x)];;
	node* currentnode;
	startnode->start_ = true;
	desnode->des_ = true;

	std::cout << "Start Node: ";
	std::cout << "X: " << startnode->nodepos_.x;
	std::cout << ",Y: " << startnode->nodepos_.y;
	std::cout << std::endl;
	std::cout << "End Node: ";
	std::cout << "X: " << desnode->nodepos_.x;
	std::cout << ",Y: " << desnode->nodepos_.y;
	std::cout << std::endl;
	std::list<node> openlist;
	std::list<node> closedlist;
	// Push starting node into front of the list
	openlist.push_front(*startnode);
	// Set current node to start of the list
	currentnode = startnode;

	while (!openlist.empty())
	{
		// For all neighbouring nodes, find least cost F node
		for (int i = 0; i < 9; i++)
		{
			node* nnode = currentnode->neighbour_[i];
			nnode->G = Vector2DDistance(nnode->nodepos_, currentnode->nodepos_);
			nnode->H = Vector2DDistance(desnode->nodepos_, nnode->nodepos_);
			nnode->F = nnode->G + nnode->H;
			if(!nnode->obstacle_ && !nnode->visited_)
				closedlist.push_front(*nnode);
		}

		// Arrange list according from lowest to highst F
		closedlist.sort([](const node lhs, const node rhs) { return lhs.F < rhs.F; });
		// Pop current node
		if(closedlist.front().G == 0)
			closedlist.pop_front();

		// No available nodes
		if (closedlist.empty())
			break;

		closedlist.front().parent_ = currentnode;
		currentnode = &closedlist.front();
		currentnode->visited_ = true;
		openlist.push_back(*currentnode);
		std::cout << currentnode->nodepos_.x << ", " <<currentnode->nodepos_.y << std::endl;
		
		// If destination hs reached
		if (currentnode->nodepos_.x == desnode->nodepos_.x &&
			currentnode->nodepos_.y == desnode->nodepos_.y)
			break;
		node_map_[currentnode->nodepos_.y][currentnode->nodepos_.x].visited_ = true;
		
		// clear list
		//if(!closedlist.empty())
		//	closedlist.clear();
	}
}

void AMap::DrawMap()
{
	std::cout << "---------------------------" << std::endl;
	for (int i = node_map_.size()-1; i >= 0; --i) {
		for (int j = 0; j < node_map_[0].size(); ++j) {
			std::cout << "|";
			//std::cout << node_map_[i][j].visited_;
			if(node_map_[i][j].visited_)
				std::cout << "*";
			else if(node_map_[i][j].start_)
				std::cout << "S";
			else if(node_map_[i][j].des_)
				std::cout << "D";
			else if (node_map_[i][j].obstacle_)
				std::cout << "X";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
}