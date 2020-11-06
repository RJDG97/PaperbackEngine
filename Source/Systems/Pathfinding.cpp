#include "Systems/Pathfinding.h"

void Pathfinding::Init()
{
	//// Allocate memory
	//nodes_ = new Node[mapwidth_ * mapheight_];

	//// Initialise map
	//for (int x = 0; x < mapwidth_; ++x)
	//{
	//	for (int y = 0; y < mapheight_; ++y)
	//	{
	//		// Set Location
	//		nodes_[y * mapwidth_ + x].x = x;
	//		nodes_[y * mapwidth_ + x].y = y;
	//		// Init variables
	//		nodes_[y * mapwidth_ + x].parent_ = nullptr;
	//		nodes_[y * mapwidth_ + x].visited_ = false;
	//		// Set Obstacle
	//		nodes_[y * mapwidth_ + x].obstacle_ = false;
	//	}
	//}

	// Connect nodes to neighbour nodes
	for (int x = 0; x < mapwidth_; ++x)
	{
		for (int y = 0; y < mapheight_; ++y)
		{
			nodes_[y * mapwidth_ + x].neighbour_.push_back(&nodes_[(y - 1) * mapwidth_ + (x)]);
		}
	}

	nstart_ = &nodes_[(mapheight_ / 2) * mapwidth_ + 1];
	nend_ = &nodes_[(mapheight_ / 2) * mapwidth_ + mapwidth_ - 2];
}

void Pathfinding::AStar()
{
	// Reset Variables for nodes
	for (int x = 0; x < mapwidth_; ++x)
	{
		for (int y = 0; y < mapheight_; ++y)
		{
			nodes_[y * mapwidth_ + x].visited_ = false;
			nodes_[y * mapwidth_ + x].globaldes_ = INFINITY;
			nodes_[y * mapwidth_ + x].localdes_ = INFINITY;
			nodes_[y * mapwidth_ + x].parent_ = nullptr;
		}
	}

	// Calculate Minimum Distance
	auto distance_ = [](Node* a, Node* b)
	{return sqrt((a->x - b->x) * (a->x - a->x) + (a->y - b->y) * (a->y - a->y)); };
	// Get Minimum Distance
	auto heuristic_ = [distance_](Node* a, Node* b)
	{return distance_(a, b); };

	// Start Condition
	Node* ncurrent_ = nstart_;
	nstart_->localdes_ = 0.0f;
	nstart_->globaldes_ = heuristic_(nstart_, nend_);

	// List of untested nodes
	std::list<Node*> untestednodelist_;
	untestednodelist_.push_back(nstart_);

	// Check if algorithm has completed (if empty then yes)
	while (!untestednodelist_.empty())
	{
		// Sort list with lowest global to highest global
		untestednodelist_.sort([](const Node* lhs, const Node* rhs) { return lhs->globaldes_ < rhs->globaldes_; });
		// Remove visited nodes
		while (!untestednodelist_.empty() && untestednodelist_.front()->visited_)
		{
			untestednodelist_.pop_front();
		}
		// No more nodes left
		if (untestednodelist_.empty())
			break;

		ncurrent_ = untestednodelist_.front();
		ncurrent_->visited_ = true;

		// Check nodes neighbours
		for (auto nneighbour_ : ncurrent_->neighbour_)
		{
			// Check if neighbour is visited or obstacle
			if (!nneighbour_->visited_ && nneighbour_->obstacle_ == 0)
				untestednodelist_.push_back(nneighbour_);

			// Calculate neighbours smallest distance
			float lowestpossiblegoal_ = ncurrent_->localdes_ + distance_(ncurrent_, nneighbour_);

			// if path has smaller distance than neighbour, update neighbour to use this path
			if (lowestpossiblegoal_ < nneighbour_->localdes_)
			{
				nneighbour_->parent_ = ncurrent_;
				nneighbour_->localdes_ = lowestpossiblegoal_;
				nneighbour_->globaldes_ = nneighbour_->localdes_ + heuristic_(nneighbour_, nend_);
			}
		}
	}
}

// Update Pathfinding
void Pathfinding::Update(float frametime)
{
	std::cout << "Pathfinding::Update" << std::endl;
	(void)frametime;
}

// Draw Nodes and paths
void Pathfinding::Draw()
{

}

// Sends Messages
void Pathfinding::SendMessageD(Message* m) {
	(void)m;
}