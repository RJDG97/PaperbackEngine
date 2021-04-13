/**********************************************************************************
*\file         Partitioning.cpp
*\brief        Contains definition of functions and variables used for
*			   the Partitioning System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
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
	aabb_map_ = component_manager_->GetComponentArray<AABB>();
	transform_map_ = component_manager_->GetComponentArray<Transform>();
	texture_map_ = component_manager_->GetComponentArray<TextureRenderer>();
	animation_map_ = component_manager_->GetComponentArray<AnimationRenderer>();
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
	renderer_x_.resize(static_cast<int>(x));
	renderer_y_.resize(static_cast<int>(y));

	abs_bottom_left_.x = abs_bottom_left_.x < 0 ? (-abs_bottom_left_.x) : abs_bottom_left_.x;
	abs_bottom_left_.y = abs_bottom_left_.y < 0 ? (-abs_bottom_left_.y) : abs_bottom_left_.y;
}


void PartitioningSystem::Update(float frametime) {
	(void)frametime;

	// Resets all the bits
	ResetPartition();

	if (x_.size() < 1 || y_.size() < 1)
		return;

	// Compute collider partitioning for 
	for (auto& [id, aabb] : *aabb_map_) {
		
		if (aabb->GetLayer() == static_cast<size_t>(CollisionLayer::BACKGROUND) ||
			aabb->GetLayer() == static_cast<size_t>(CollisionLayer::UI_ELEMENTS))
			continue;

		InitEntityInPartition(id);
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



const PartitioningSystem::EntityIDSet& PartitioningSystem::GetActiveEntityIDs() const {
	
	return id_set_;
}


// Call this function every loop
void PartitioningSystem::ComputePartitionedEntities() {

	float inv_cam_zoom{};
	Bitset all_entities{};
	Vector2D bottom_left{}, top_right{}, camera_pos{};
	My::Vector2D<size_t> my_bottom_left{}, my_top_right{};
	Camera* camera = nullptr;
	Transform* transform = nullptr;

	camera = CORE->GetSystem<CameraSystem>()->GetMainCamera();

	// If camera doesn't exist, skip computation
	if (!camera)
		return;

	inv_cam_zoom = 1 / *camera->GetCameraZoom();
	transform = component_manager_->GetComponent<Transform>(camera->GetOwner()->GetID());

	// If transform doesn't exist for camera, skip computation - To be changed eventually when camera is not an entity
	if (!transform)
		return;

	// Clear the IDSet
	id_set_.clear();

	// Convert camera's position from GameScale to WorldScale
	camera_pos = transform->GetOffsetAABBPos() * CORE->GetGlobalScale();

	// Compute the bottom left and top right of the area to be displayed
	ComputeBoundaries(camera_pos, inv_cam_zoom, bottom_left, top_right);
	// Convert the positions to game coordinates (Divide by GameScale)
	ConvertBoundariesToLocal(bottom_left, top_right);
	// Convert the positions to grid coordinates (Divide by PartitionScale)
	ComputePartitionBoundaries(bottom_left, top_right);

	// Cast to size_t
	my_bottom_left.x = static_cast<size_t>(bottom_left.x);
	my_bottom_left.y = static_cast<size_t>(bottom_left.y);
	my_top_right.x = static_cast<size_t>(top_right.x);
	my_top_right.y = static_cast<size_t>(top_right.y);

	// Iterate through partition boundaries to determine all entities that lie within
	for (size_t i = my_bottom_left.y; i < my_top_right.y; ++i) {
		for (size_t j = my_bottom_left.x; j < my_top_right.x; ++j) {

			if (i >= renderer_y_.size() || j >= renderer_x_.size()) {
				
				M_DEBUG->WriteDebugMessage("Partitioning Bitset: " + 
					std::to_string(renderer_x_.size()) + " " + 
					std::to_string(renderer_y_.size()) + "\n");
				return;
			}
		
			// Perform an & to determine common entities within both areas
			// To allow for checking of values
			Bitset x = renderer_x_[j];
			Bitset y = renderer_y_[i];
			all_entities |= x & y;
		}
	}

	// If an EntityID is set, insert it's ID into the std::set to be marked as active
	for (size_t i = 0; i < all_entities.size(); ++i) {
		
		if (all_entities.test(i)) {

			id_set_.insert(i);
		}
	}
}

void PartitioningSystem::ComputeBoundaries(const Vector2D& camera_pos, const float& camera_zoom, Vector2D& bottom_left, Vector2D& top_right) {

	WindowsSystem* windows = &*CORE->GetSystem<WindowsSystem>();

	if (windows) {

		bottom_left.x = camera_pos.x - (windows->GetWinWidth() * 0.5f * camera_zoom);
		bottom_left.y = camera_pos.y - (windows->GetWinHeight() * 0.5f * camera_zoom);

		top_right.x = camera_pos.x + (windows->GetWinWidth() * 0.5f * camera_zoom);
		top_right.y = camera_pos.y + (windows->GetWinHeight() * 0.5f * camera_zoom);
	}
	
	DEBUG_ASSERT(windows, "WindowsSystem does not exist!!");
}

void PartitioningSystem::ConvertBoundariesToLocal(Vector2D& bottom_left, Vector2D& top_right) {

	// Assumption is that GlobalScale will NEVER be 0
	float inv_scale = 1 / CORE->GetGlobalScale();

	// Convert to game coordinates by dividing by GameScale
	bottom_left *= inv_scale;
	top_right *= inv_scale;

	// Shift the coordinates so that the minimum will always be (0, 0) or greater
	bottom_left += abs_bottom_left_;
	top_right += abs_bottom_left_;
}

void PartitioningSystem::ComputePartitionBoundaries(Vector2D& bottom_left, Vector2D& top_right) {

	float inv_partition_scale = 1.0f / grid_size_;

	// Convert the game coordinates into partition grid
	bottom_left *= inv_partition_scale;
	top_right *= inv_partition_scale;

	// Round values to extreme boundaries to cover nearby entities
	RoundDown(bottom_left.x);
	RoundDown(bottom_left.y);
	RoundUp(top_right.x);
	RoundUp(top_right.y);

	// Make sure smallest values are set to the bottom for iteration purposes
	if (bottom_left.x > top_right.x)
		std::swap(bottom_left.x, top_right.x);
	if (bottom_left.y > top_right.y)
		std::swap(bottom_left.y, top_right.y);
}

void PartitioningSystem::InitEntityInPartition(const EntityID& id) {

	float min_x, min_y, max_x, max_y, pos_x, pos_y;

	Transform* xform = transform_map_->GetComponent(id);
	AABB* aabb = aabb_map_->GetComponent(id);

	if (!xform || !aabb)
		return;

	// Computing the position of the entity
	pos_x = xform->GetOffsetAABBPos().x + abs_bottom_left_.x;
	pos_y = xform->GetOffsetAABBPos().y + abs_bottom_left_.y;

	Vector2D aabb_scale_ = aabb->GetAABBScale();

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
			x_[j].set(id);
	}
	for (int i = static_cast<int>(min_y); i < max_y; ++i) {
		if (i >= 0 && i < y_.size())
			y_[i].set(id);
	}
}

void PartitioningSystem::InitRendererInPartition(const EntityID& id) {

	Vector2D pos{}, min{}, max{};
	Transform* xform = component_manager_->GetComponent<Transform>(id);
	Scale* scale = component_manager_->GetComponent<Scale>(id);

	if (!xform || !scale) return;

	// Computing the position of the entity
	pos.x = xform->GetOffsetAABBPos().x + abs_bottom_left_.x;
	pos.y = xform->GetOffsetAABBPos().y + abs_bottom_left_.y;

	Vector2D texture_scale_ = scale->GetScale();

	// Computing the min and max pos of the entity
	min.x = (pos.x - texture_scale_.x) / grid_size_;
	RoundDown(min.x);
	min.y = (pos.y - texture_scale_.y) / grid_size_;
	RoundDown(min.y);
	max.x = (pos.x + texture_scale_.x) / grid_size_;
	RoundUp(max.x);
	max.y = (pos.y + texture_scale_.y) / grid_size_;
	RoundUp(max.y);

	// Setting the bits for the grid based on entity location
	for (int j = static_cast<int>(min.x); j < max.x; ++j) {
		if (j >= 0 && j < renderer_x_.size())
			renderer_x_[j].set(id);
	}
	for (int i = static_cast<int>(min.y); i < max.y; ++i) {
		if (i >= 0 && i < renderer_y_.size())
			renderer_y_[i].set(id);
	}
}