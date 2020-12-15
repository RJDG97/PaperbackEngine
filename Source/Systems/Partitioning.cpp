/**********************************************************************************
*\file         Partitioning.cpp
*\brief        Contains definition of functions and variables used for
*			   the Partitioning System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Engine/Core.h"
#include "Manager/AMap.h"
#include "Systems/Partitioning.h"
#include "Systems/Collision.h"
#include <algorithm>

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

	// Resets all the bits
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
		pos_x = xform->GetOffsetAABBPos().x + abs_bottom_left_.x;
		pos_y = xform->GetOffsetAABBPos().y + abs_bottom_left_.y;

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

	ComputePartitionedEntities();
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



const PartitioningSystem::EntityIDSet& PartitioningSystem::GetActiveEntityIDs() {
	
	return id_set_;
}


// Call this function every loop
void PartitioningSystem::ComputePartitionedEntities() {

	Bitset all_entities{};
	My::Vector2D<size_t> my_bottom_left{}, my_top_right{};
	Vector2D bottom_left{}, top_right{}, camera_pos{};
	Camera* camera = nullptr;



	// Little tester
	CMap<Camera>* camera_arr_ = component_manager_->GetComponentArray<Camera>();
	if (camera_arr_->begin() != camera_arr_->end())
		camera = camera_arr_->begin()->second;



	if (!camera)
		return;

	// Clear the IDSet every loop
	id_set_.clear();

	camera_pos = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID())->GetOffsetAABBPos() * CORE->GetGlobalScale() + abs_bottom_left_;

	// Compute the bottom left and top right of the area to be displayed
	ComputeBoundaries(camera_pos, bottom_left, top_right);
	// 
	ConvertBoundariesToLocal(bottom_left, top_right);
	ComputePartitionBoundaries(bottom_left, top_right);


	// Cast to size_t
	my_bottom_left.x = static_cast<size_t>(bottom_left.x);
	my_bottom_left.y = static_cast<size_t>(bottom_left.y);
	my_top_right.x = static_cast<size_t>(top_right.x);
	my_top_right.y = static_cast<size_t>(top_right.y);


	for (size_t i = my_bottom_left.y; i < my_top_right.y; ++i) {
		for (size_t j = my_bottom_left.x; j < my_top_right.x; ++j) {

			if (i >= y_.size() || j >= x_.size()) {
				
				M_DEBUG->WriteDebugMessage("Partitioning Bitset: " + std::to_string(x_.size()) + " " + std::to_string(y_.size()) + "\n");
				return;
			}
		
			// Perform an & to determine common entities within both areas
			// To allow for checking of values
			Bitset x = x_[j];
			Bitset y = y_[i];
			all_entities |= x & y;
		}
	}

	// If bit is |'d, insert into the unordered_set to be iterated through
	for (size_t i = 0; i < all_entities.size(); ++i) {
		
		if (all_entities.test(i)) {

			id_set_.insert(i);
		}
	}
}

void PartitioningSystem::ComputeBoundaries(const Vector2D& camera_pos, Vector2D& bottom_left, Vector2D& top_right) {

	std::shared_ptr<WindowsSystem> windows = CORE->GetSystem<WindowsSystem>();
	

	if (windows) {

		bottom_left.x = camera_pos.x - (windows->GetWinWidth() * 0.5f);
		bottom_left.y = camera_pos.y - (windows->GetWinHeight() * 0.5f);

		top_right.x = camera_pos.x + (windows->GetWinWidth() * 0.5f);
		top_right.y = camera_pos.y + (windows->GetWinHeight() * 0.5f);
	}
	else {
		
		// Essentially an error
		bottom_left.x = -1.0f;
	}
}

void PartitioningSystem::ConvertBoundariesToLocal(Vector2D& bottom_left, Vector2D& top_right) {

	if (bottom_left.x == -1.0f)
		return;

	// Assumption is that GlobalScale will NEVER be 0
	float inv_scale = 1 / CORE->GetGlobalScale();

	bottom_left *= inv_scale;
	top_right *= inv_scale;
}

void PartitioningSystem::ComputePartitionBoundaries(Vector2D& bottom_left, Vector2D& top_right) {

	if (bottom_left.x == -1.0f)
		return;

	float inv_partition_scale = 1.0f / grid_size_;
	float inv_scale = 1 / CORE->GetGlobalScale();

	bottom_left *= inv_partition_scale;
	top_right *= inv_partition_scale;

	// Round values to extreme boundaries to cover nearby entities
	RoundDown(bottom_left.x);
	RoundDown(bottom_left.y);
	RoundUp(top_right.x);
	RoundUp(top_right.y);

	// Map coordinates to positive axes
	if (bottom_left.x < 0.0f) {

		float x_boundary = CORE->GetManager<AMap>()->GetBottomLeft().x * inv_scale;

		if (bottom_left.x < x_boundary) {
			bottom_left.x = std::max(x_boundary, bottom_left.x) - std::min(x_boundary, bottom_left.x);
		}

		top_right.x += abs(bottom_left.x);
		//bottom_left.x = 0.0f;
	}
	if (bottom_left.y < 0.0f) {
		
		float y_boundary = CORE->GetManager<AMap>()->GetBottomLeft().y * inv_scale;

		if (bottom_left.y < y_boundary) {
			bottom_left.y = std::max(y_boundary, bottom_left.y) - std::min(y_boundary, bottom_left.y);
			//bottom_left.y = y_boundary;
		}

		top_right.y += abs(bottom_left.y);
		//bottom_left.y = 0.0f;
	}

	if (bottom_left.x > top_right.x)
		std::swap(bottom_left.x, top_right.x);
	if (bottom_left.y > top_right.y)
		std::swap(bottom_left.y, top_right.y);
}