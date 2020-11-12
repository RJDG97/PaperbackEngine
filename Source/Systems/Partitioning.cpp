#include "Engine/Core.h"
#include "Manager/AMap.h"
#include "Systems/Partitioning.h"
#include "Systems/Collision.h"

void RoundDown(float& var) {
	
	int tmp = static_cast<int>(var);
	var = static_cast<float>((var < tmp) ? static_cast<int>(var) - 1 : static_cast<int>(var));
}


void RoundUp(float& var) {

	int tmp = static_cast<int>(var);
	var = static_cast<float>((var < tmp) ? static_cast<int>(var) : static_cast<int>(var) + 1);
}

void PartitioningSystem::Init() {
	
	component_manager_ = &*CORE->GetManager<ComponentManager>();
	transform_map_ = component_manager_->GetComponentArray<Transform>();
	aabb_map_ = component_manager_->GetComponentArray<AABB>();
	grid_size_ = 4;

	InitPartition();
}

void PartitioningSystem::InitPartition() {

	abs_bottom_left_ = CORE->GetManager<AMap>()->GetBottomLeft();
	abs_top_right_ = CORE->GetManager<AMap>()->GetTopRight();

	//int x = static_cast<int>((abs_top_right_.x - abs_bottom_left_.x) / grid_size_);
	//int y = static_cast<int>((abs_top_right_.y - abs_bottom_left_.y) / grid_size_);

	float x = (abs_top_right_.x - abs_bottom_left_.x) / grid_size_;
	float y = (abs_top_right_.y - abs_bottom_left_.y) / grid_size_;

	RoundUp(x);
	RoundUp(y);

	x_.resize( static_cast<int>(x) );
	y_.resize( static_cast<int>(y) );

	abs_bottom_left_.x = abs_bottom_left_.x < 0 ? (-abs_bottom_left_.x) : abs_bottom_left_.x;
	abs_bottom_left_.y = abs_bottom_left_.y < 0 ? (-abs_bottom_left_.y) : abs_bottom_left_.y;
}


void PartitioningSystem::Update(float frametime) {
	(void)frametime;

	ResetPartition();

	if (x_.size() < 1 || y_.size() < 1)
		return;

	for (AABBMapIt it = aabb_map_->begin(); it != aabb_map_->end(); ++it) {
		
		if (it->second->GetLayer() == static_cast<size_t>(CollisionLayer::BACKGROUND) ||
			it->second->GetLayer() == static_cast<size_t>(CollisionLayer::UI_ELEMENTS))
			continue;

		//int min_x, min_y, max_x, max_y;
		float min_x, min_y, max_x, max_y, pos_x, pos_y;
		
		Transform* xform = transform_map_->GetComponent(it->first);

		if (!xform)
			continue;

		// Computing the position of the entity
		pos_x = xform->GetPosition().x + abs_bottom_left_.x;
		pos_y = xform->GetPosition().y + abs_bottom_left_.y;

		Vector2D aabb_scale_ = it->second->GetAABBScale();

		// Computing the min and max pos of the entity
		min_x = (pos_x - aabb_scale_.x) / grid_size_;
		RoundDown(min_x);
		min_y = (pos_y - aabb_scale_.y) / grid_size_;
		RoundDown(min_y);
		max_x = (pos_x + aabb_scale_.x) / grid_size_;
		RoundUp(max_x);
		max_y = (pos_y + aabb_scale_.y) / grid_size_;
		RoundUp(max_y);

		// Setting the bits for the grid based on entity location
		for (int j = static_cast<int>(min_x); j < max_x; ++j) {
			if (j >= 0 && j < x_.size())
				x_[j].set(it->first);
		}
		for (int i = static_cast<int>(min_y); i < max_y; ++i) {
			if (i >= 0 && i < y_.size())
				y_[i].set(it->first);
		}
	}
}


void PartitioningSystem::SendMessageD(Message* m) {
	(void)m;
}


PartitioningSystem::Bitset PartitioningSystem::GetCoordBitset(size_t x, size_t y) const {
	
	return (x_.size() > x && y_.size() > y) ? 
		(x_[x] & y_[y]) : 
		Bitset{};
}

void PartitioningSystem::ResetPartition() {

	for (Bitset& x : x_) {
		
		x.reset();
	}

	for (Bitset& y : y_) {
		
		y.reset();
	}
}

void PartitioningSystem::GetPartitionedEntities(std::vector<AABBMapIt>& vec, size_t x, size_t y) {
	
	// If out of bounds (Too big or too small)
	// or there is fewer than 1 entity within the partition
	if ((x >= x_.size() || y >= y_.size()) || (x < 0 || y < 0) && 
		(x_[x].count() < 1 || y_[y].count() < 1)) {
		vec.clear();
		return;
	}

	Bitset entities_within_ = x_[x] & y_[y];

	for (size_t i = 0; i < entities_within_.size(); ++i) {

		if (entities_within_.test(i)) {
			
			vec.push_back( aabb_map_->GetComponentIt(i) );
		}
	}
}

bool PartitioningSystem::VerifyPartition(size_t x, size_t y) {
	
	if ((x >= x_.size() || y >= y_.size()) || (x < 0 || y < 0)) {

		return false;
	}

	Bitset entities_within = x_[x] & y_[y];

	return (entities_within.count() > 1);
}

std::pair<size_t, size_t> PartitioningSystem::GetAxisSizes() {
	x_ = x_;
	return { x_.size(), y_.size() };
	
}

Vector2D PartitioningSystem::ConvertTransformToGridScale(const Vector2D& pos) {
	
	Vector2D grid_coords = pos;

	grid_coords += abs_bottom_left_;
	grid_coords /= static_cast<float>(grid_size_);

	RoundDown(grid_coords.x);
	RoundDown(grid_coords.y);

	return grid_coords;
}