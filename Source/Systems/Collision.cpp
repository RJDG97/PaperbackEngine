#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/Collision.h"
#include "Systems/Factory.h"
#include "Systems/Game.h"
#include "Systems/Debug.h"
#include "Systems/InputSystem.h"
#include "Manager/ForcesManager.h"
#include "Components/Transform.h"
#include "Entity/ComponentCreator.h"
#include "Entity/ComponentTypes.h"
#include "Components/AABB.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include "Components/Scale.h"
#include "Components/Inventory.h"
#include <iostream>
#include <assert.h>
#include <glm/gtc/type_ptr.hpp>
#include <future>

#define EPSILON 0.001f

Collision* COLLISION;

Collision::Collision() {

	debug_ = false;
}

// Comparison function
auto max = [](float a, float b) {
	return (a > b) ? a : b;
};

auto min = [](float a, float b) {
	return (a < b) ? a : b;
};

auto absVec = [](Vector2D vec) {

	vec.x = abs(vec.x);
	vec.y = abs(vec.y);

	return vec;
};

// Instead of scale* use aabb's custom scale
Vector2D Normal(const Collision::AABBIt& s1, const Collision::AABBIt& s2,
	const Transform* t1, const Transform* t2) {

	Vector2D scale_sum = s1->second->GetAABBScale() + s2->second->GetAABBScale();
	Vector2D vec = absVec(t1->GetOffsetAABBPos() - t2->GetOffsetAABBPos());

	Vector2D minkowski;
	minkowski.x = vec.y * scale_sum.x;
	minkowski.y = vec.x * scale_sum.y;


	if (minkowski.y > minkowski.x) {
		if (minkowski.y > -minkowski.x) {
			// Collision on top for Object A
			return Vector2D{ 1, 0 };
		}
		else {
			// Collision on left for Object A
			return Vector2D{ 0, 1 };
		}
	}
	else {
		if (minkowski.y > -minkowski.x) {
			// Collision on the right for Object A
			return Vector2D{ 0, 1 };
		}
		else {
			// Collision on the bottom for Object A
			return Vector2D{ 1, 0 };
		}
	}
}

Vector2D SATNormal(const Collision::AABBIt& s1, const Collision::AABBIt& s2,
	const Transform* t1, const Transform* t2, float& penetration) {

	Vector2D vec_ba = t2->GetOffsetAABBPos() - t1->GetOffsetAABBPos();

	float x_over = abs(vec_ba.x) - (s1->second->GetAABBScale().x + s2->second->GetAABBScale().x);

	float y_over = abs(vec_ba.y) - (s1->second->GetAABBScale().y + s2->second->GetAABBScale().y);

	float diff = x_over - y_over;

	if (diff > EPSILON) {

		penetration = x_over;

		if (vec_ba.x < 0.0f) {

			return Vector2D{ -1.0f, 0.0f };
		}
		else {

			return Vector2D{ 1.0f, 0.0f };
		}
	}
	else if (diff < -EPSILON) {

		penetration = y_over;

		if (vec_ba.y < 0.0f) {

			return Vector2D{ 0.0f, -1.0f };
		}
		else {

			return Vector2D{ 0.0f, 1.0f };
		}
	}
	else {

		penetration = x_over;
		Vector2D normal{};

		normal.x = (vec_ba.x < 0.0f) ? -1.0f : 1.0f;

		normal.y = (vec_ba.y < 0.0f) ? -1.0f : 1.0f;

		return normal;
	}
}

void Collision::AddCollisionLayers(CollisionLayer layer, const std::string& collidables, bool collide_self) {
	CollidableLayers collider{ collidables, collide_self };

	collision_layer_arr_.insert(std::make_pair(layer, collider));
}

bool Collision::CheckCollision(const AABB& aabb1, const Vec2& vel1,
	const AABB& aabb2, const Vec2& vel2,
	const float dt, float& tFirst) {

	// AABB_1
	Vector2D aab1_bot_left = aabb1.GetBottomLeft();
	Vector2D aab1_top_right = aabb1.GetTopRight();

	// AABB_2
	Vector2D aab2_bot_left = aabb2.GetBottomLeft();
	Vector2D aab2_top_right = aabb2.GetTopRight();
	float tLast = dt; // g_dt does not exist yet
	tFirst = {};

	if (SeparatingAxisTheorem(aabb1, aabb2))
	{
		Vector2D Vb;
		float t_first_x{}, t_last_x = tLast;

		Vb.x = vel2.x - vel1.x;
		Vb.y = vel2.y - vel1.y;

		if ((Vb.x < EPSILON && Vb.x > -EPSILON) || (Vb.y > -EPSILON && Vb.y < EPSILON))
			return 0;

		// X-Axis check
		if (Vb.x < -EPSILON)
		{
			//case 1
			if (aab1_bot_left.x > aab2_top_right.x)
				return 0;
			//case 4
			if (aab1_top_right.x < aab2_bot_left.x)
				t_first_x = max(abs((aab1_top_right.x - aab2_bot_left.x) / Vb.x), t_first_x);
			if (aab1_bot_left.x < aab2_top_right.x)
				t_last_x = min(abs((aab1_bot_left.x - aab2_top_right.x) / Vb.x), t_last_x);
		}
		if (Vb.x > EPSILON)
		{
			//case 2
			if (aab1_bot_left.x > aab2_top_right.x)
				t_first_x = max(abs((aab1_bot_left.x - aab2_top_right.x) / Vb.x), t_first_x);
			if (aab1_top_right.x > aab2_bot_left.x)
				t_last_x = min(abs((aab1_top_right.x - aab2_bot_left.x) / Vb.x), t_last_x);
			//case3
			if (aab1_top_right.x < aab2_bot_left.x)
				return 0;
		}
		if (t_first_x > t_last_x)
			return 0;

		float t_first_y{}, t_last_y = tLast;

		// Y-Axis check
		if (Vb.y < -EPSILON)
		{
			//case 1
			if (aab1_bot_left.y > aab2_top_right.y)
				return 0;
			//case 4
			if (aab1_top_right.y < aab2_bot_left.y)
				t_first_y = max(abs((aab1_top_right.y - aab2_bot_left.y) / Vb.y), t_first_y);
			if (aab1_bot_left.y < aab2_top_right.y)
				t_last_y = min(abs((aab1_bot_left.y - aab2_top_right.y) / Vb.y), t_last_y);
		}
		if (Vb.y > EPSILON)
		{
			//case 2
			if (aab1_bot_left.y > aab2_top_right.y)
				t_first_y = max(abs((aab1_bot_left.y - aab2_top_right.y) / Vb.y), t_first_y);
			if (aab1_top_right.y > aab2_bot_left.y)
				t_last_y = min(abs((aab1_top_right.y - aab2_bot_left.y) / Vb.y), t_last_y);
			//case3
			if (aab1_top_right.y < aab2_bot_left.y)
				return 0;
		}
		if (t_first_y > t_last_y)
			return 0;

		tFirst = (t_first_x > t_first_y) ? t_first_x : t_first_y;
	}
	return 1;
}

bool VerifyCursorCollision(const Vector2D& bottom_left, const Vector2D& top_right, const Vector2D& cursor_pos) {

	//compute if position is within bounding box
	if (bottom_left.x <= cursor_pos.x &&
		bottom_left.y <= cursor_pos.y &&
		top_right.x >= cursor_pos.x &&
		top_right.y >= cursor_pos.y) {
		return true;
	}
	return false;
}

bool Collision::CheckCursorCollision(const Vec2& cursor_pos, const Clickable* button) {

	Vec2 cursor_pos_scaled = cursor_pos;

	//convert button AABB to global
	Vec2 bottom_left = CORE->GetGlobalScale() * button->bottom_left_;
	Vec2 top_right = CORE->GetGlobalScale() * button->top_right_;

	//assume that is button
	return VerifyCursorCollision(bottom_left, top_right, cursor_pos_scaled);
}

bool Collision::CheckCursorCollision(const Vec2& cursor_pos, const AABB* box) {

	//Vec2 cursor_pos_scaled = (1 / *camera_->GetMainCamera()->GetCameraZoom()) * cursor_pos;
	
	//convert button AABB to global
	Vec2 bottom_left = box->bottom_left_;
	Vec2 top_right = box->top_right_;

	return VerifyCursorCollision(bottom_left, top_right, cursor_pos);
}

//std::pair<Entity*, std::vector<ComponentTypes>> Collision::GetAttachedComponentIDs() {
//	std::vector<ComponentTypes> comp_arr;
//	Entity* entity;
//	EntityID selected_entity_id = SelectEntity();
//
//	if (selected_entity_id != 0) {
//		// Grab entity from factory and return the bitset
//		std::shared_ptr<EntityFactory> factory = CORE->GetSystem<EntityFactory>();
//		entity = factory->GetObjectWithID(selected_entity_id);
//
//		// If entity exists in factory
//		if (entity) {
//			// Grab component array from entity
//			ComponentArr arr = entity->GetComponentArr();
//			// Resize component array accordingly
//			comp_arr.reserve(arr.size());
//
//			// Iterate all components and save the enums
//			for (ComponentArrIt it = arr.begin(); it != arr.end(); ++it) {
//				comp_arr.push_back((*it)->GetComponentTypeID());
//			}
//		}
//	}
//
//	// Return compiled entity 
//	return std::make_pair(entity, comp_arr);
//}

bool Collision::SeparatingAxisTheorem(const AABB& a, const AABB& b) {
	// AABB_1
	Vector2D aab1_bot_left = a.GetBottomLeft();
	Vector2D aab1_top_right = a.GetTopRight();

	// AABB_2
	Vector2D aab2_bot_left = b.GetBottomLeft();
	Vector2D aab2_top_right = b.GetTopRight();

	// Check if there is at least an axis that is intersecting
	if ((aab1_bot_left.x > aab2_top_right.x || aab1_top_right.x < aab2_bot_left.x) ||
		(aab1_bot_left.y > aab2_top_right.y || aab1_top_right.y < aab2_bot_left.y)) {
		return 1;
	}

	return 0;
}

void Collision::CheckClickableCollision() {

	Vector2D cursor_pos = CORE->GetSystem<InputSystem>()->GetCursorPosition();

	for (ClickableIt clickable = clickable_arr_->begin(); clickable != clickable_arr_->end(); ++clickable) {

		// Only if clickable is set to active
		if (clickable->second->active_) {

			if (CheckCursorCollision(cursor_pos, clickable->second)) {

				Message_Button msg{ clickable->second->index_ };
				CORE->BroadcastMessage(&msg);
				return;
			}
		}
	}
}

EntityID Collision::SelectEntity(Vector2D cursor_pos) {
		// Iterate through the external layer map to access all AABB components on that "Layer"
		for (CollisionMapReverseIt it1 = collision_map_.rbegin(); it1 != collision_map_.rend(); ++it1) {
			// Iterate through the internal layer map to access each individual AABB component
			for (AABBIt it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {

				if (CheckCursorCollision(cursor_pos, it2->second)) {
					return it2->second->GetOwner()->GetID();
				}
			}
		}
	return 0;
}

void Collision::DefaultResponse(AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2, float frametime, float t_first) {

	//std::shared_ptr<ForcesManager> force_mgr = CORE->GetManager<ForcesManager>();
	UNREFERENCED_PARAMETER(vel2);
	Vector2D inverse_vector_1 = (-(*vel1)) * (frametime - t_first);
	//Vector2D inverse_vector_2 = (-(*vel2)) * (frametime - t_first);

	Transform* transform1 = transform_arr_->GetComponent(aabb1->first);
	Transform* transform2 = transform_arr_->GetComponent(aabb2->first);

	float penetration{};

	// Get "normal" to colliding side
	Vector2D normal = SATNormal(aabb1, aabb2, transform1, transform2, penetration);

	inverse_vector_1.x *= abs(normal.x);
	inverse_vector_1.y *= abs(normal.y);
	//inverse_vector_2.x *= abs(normal.x);
	//inverse_vector_2.y *= abs(normal.y);

	normal *= abs(penetration) * 0.7f;

	// Reposition entity's position
	transform1->position_ += inverse_vector_1;
	transform2->position_ += normal; //inverse_vector_2;

	// Toggle collision status (For debug boxes)
	aabb1->second->collided = true;
	aabb2->second->collided = true;

	if (debug_) {
		std::string debug_str{};
		debug_str += "time to collision: " + std::to_string(t_first) + "\n"
			+ "Inverse vector 1: " + std::to_string(inverse_vector_1.x) + ", "
			+ std::to_string(inverse_vector_1.y) + "\n";
		M_DEBUG->WriteDebugMessage(debug_str);
	}
}

void Collision::WallvEnemyResponse(AABBIt aabb1, AABBIt aabb2) {

	UNREFERENCED_PARAMETER(aabb1);
	AI* ai_state = component_mgr_->GetComponent<AI>(aabb2->first);
	if (ai_state->GetState() == AI::AIState::Attack)
	{
		ai_state->SetState(AI::AIState::Withdraw);
	}
}


bool Collision::PlayervEnemyResponse(AABBIt aabb1, AABBIt aabb2) {
	Status* player_status = status_arr_->GetComponent(aabb2->first);;

	if (player_status) {

		if (player_status->status_ == StatusType::NONE) {

			// Set collided status
			aabb1->second->collided = true;
			aabb2->second->collided = true;
			Health* player_health = component_mgr_->GetComponent<Health>(aabb2->first);

			// Throw an exception if player does not have a health component
			DEBUG_ASSERT(player_health, "Player does not have a health component!");

			player_status->status_ = StatusType::HIT;
			player_status->status_timer_ = 2.0f;
			--(player_health->current_health_);
			return true;
		}

		else if (player_status->status_ == StatusType::BURROW) {
			return false;
		}

		else if (player_status->status_ == StatusType::INVISIBLE) {

			// Check this return type again when mechanics are confirmed
			return false;
		}

	}

	// Placeholder, should not be a need for this
	return false;
}

void Collision::GoalResponse() {

	Message msg{ MessageIDTypes::GSM_WIN };
	CORE->BroadcastMessage(&msg);
}

bool Collision::PlayerGateResponse(AABBIt aabb1, AABBIt aabb2) {

	Inventory* player_inven = component_mgr_->GetComponent<Inventory>(aabb1->first);

	if (player_inven && player_inven->HasKey()) {

		player_inven->SetHasKey(false);
		FACTORY->Destroy(aabb2->second->GetOwner());

		return false;
	}

	return true;
}

void Collision::PlayerKeyResponse(AABBIt aabb1, AABBIt aabb2) {

	Inventory* player_inven = component_mgr_->GetComponent<Inventory>(aabb1->first);

	if (player_inven) {

		player_inven->SetHasKey(true);
		FACTORY->Destroy(aabb2->second->GetOwner());

		return;
	}
}

void Collision::PlayerCollectibleResponse(AABBIt aabb1, AABBIt aabb2){
	
	Health* player_health = component_mgr_->GetComponent<Health>(aabb1->first);
	if (player_health->current_health_ != player_health->GetMaxHealth())
	{
		++(player_health->current_health_);
		FACTORY->Destroy(aabb2->second->GetOwner());
	}

}

void Collision::CollisionResponse(const CollisionLayer& layer_a, const CollisionLayer& layer_b,
	AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2,
	float frametime, float t_first) {

	switch (layer_a)
	{
	case CollisionLayer::TILES:
	{
		switch (layer_b)
		{
			case CollisionLayer::ENEMY:
			{
				WallvEnemyResponse(aabb1, aabb2);
			}
		}
		DefaultResponse(aabb1, vel1, aabb2, vel2, frametime, t_first);
		break;
	}
	case CollisionLayer::ENEMY:
	{
		AI* ai_state = component_mgr_->GetComponent<AI>(aabb1->first);
		if (!ai_state->GetLife())
			break;

		switch (layer_b)
		{
		case CollisionLayer::PLAYER:
		{
			if (PlayervEnemyResponse(aabb1, aabb2)) {

				// Comment this if you would like players to be able to phase through enemies
				// when colliding and status is set to "Hit" (Player invulnerable for a set time)
				DefaultResponse(aabb1, vel1, aabb2, vel2, frametime, t_first);
			}
			break;
		}
		case CollisionLayer::TILES:
		{

			DefaultResponse(aabb1, vel1, aabb2, vel2, frametime, t_first);
		}
		}
		break;
	}
	case CollisionLayer::PLAYER:
	{
		switch (layer_b)
		{
			case CollisionLayer::GOAL:
			{
				GoalResponse();
				break;
			}
			case CollisionLayer::KEYS: 
			{

				PlayerKeyResponse(aabb1, aabb2);
				break;
			}
			case CollisionLayer::GATE:
			{
				if (PlayerGateResponse(aabb1, aabb2)) {
					
					//if fails to open gate then prevent player from going past
					DefaultResponse(aabb1, vel1, aabb2, vel2, frametime, t_first);
				}
				break;
			}
			case CollisionLayer::COLLECTIBLE:
			{
				PlayerCollectibleResponse(aabb1, aabb2);
				break;
			}
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void Collision::ProcessCollision(CollisionMapIt col_layer_a, CollisionMapIt col_layer_b, float frametime) {

	for (AABBIt layer_a_it = col_layer_a->second.begin(); layer_a_it != col_layer_a->second.end(); ++layer_a_it) {
		for (AABBIt layer_b_it = col_layer_b->second.begin(); layer_b_it != col_layer_b->second.end(); ++layer_b_it) {

			//skip if same 
			if (layer_a_it->first == layer_b_it->first)
				continue;

			Vector2D vel1 = motion_arr_->GetComponent(layer_a_it->first) ?
				motion_arr_->GetComponent(layer_a_it->first)->velocity_ : Vector2D{};
			Vector2D vel2 = motion_arr_->GetComponent(layer_b_it->first) ?
				motion_arr_->GetComponent(layer_b_it->first)->velocity_ : Vector2D{};

			float t_first{};

			if (CheckCollision(*layer_a_it->second, vel1, *layer_b_it->second, vel2, frametime, t_first)) {

				AABBIt aabb1 = layer_a_it;
				AABBIt aabb2 = layer_b_it;

				CollisionResponse(col_layer_a->first, col_layer_b->first, aabb1, &vel1, aabb2, &vel2, frametime, t_first);
			}
		}
	}
}

void Collision::AddAABBComponent(EntityID id, AABB* aabb) {

	M_DEBUG->WriteDebugMessage("Adding AABB Component to entity: " + std::to_string(id) + "\n");

	//aabb_arr_[id] = aabb;

	collision_map_[static_cast<CollisionLayer>(aabb->layer_)].insert({ id, aabb });
}

void Collision::RemoveAABBComponent(EntityID id) {

	for (CollisionMapIt begin = collision_map_.begin(); begin != collision_map_.end(); ++begin) {
		AABBIt comp = begin->second.find(id);
		if (comp != begin->second.end()) {


			M_DEBUG->WriteDebugMessage("Removing AABB Component from map: " + std::to_string(id) + "\n");
			begin->second.erase(comp);
			break;
		}
	}
}

void Collision::UpdateBoundingBox() {
	if (debug_)
		M_DEBUG->WriteDebugMessage("Collision System: Updating Bounding Boxes\n");

	for (CollisionMapIt it = collision_map_.begin(); it != collision_map_.end(); ++it) {
		for (AABBIt aabb = it->second.begin(); aabb != it->second.end(); ++aabb) {

			//reset collided flag to false to prepare for collision check after
			aabb->second->collided = false;

			Entity* entity = aabb->second->GetOwner();
			DEBUG_ASSERT((entity), "Entity does not exist");

			Transform* entity_position = transform_arr_->GetComponent(entity->GetID());

			aabb->second->bottom_left_ = entity_position->GetOffsetAABBPos() - aabb->second->scale_;
			aabb->second->top_right_ = entity_position->GetOffsetAABBPos() + aabb->second->scale_;
		}
	}
}

void Collision::UpdateClickableBB() {
	if (debug_)
		M_DEBUG->WriteDebugMessage("Collision System: Updating Clickable Bounding Boxes\n");

	for (ClickableIt clickable = clickable_arr_->begin(); clickable != clickable_arr_->end(); ++clickable) {

		//reset collided flag to false to prepare for collision check after
		clickable->second->collided_ = false;

		Entity* entity = clickable->second->GetOwner();
		DEBUG_ASSERT((entity), "Entity does not exist");

		Transform* entity_position = transform_arr_->GetComponent(clickable->second->GetOwner()->GetID());

		clickable->second->bottom_left_ = entity_position->position_ - clickable->second->scale_;
		clickable->second->top_right_ = entity_position->position_ + clickable->second->scale_;
	}
}

bool Collision::BurrowReady() {
	
	Entity* player_entity = entity_mgr_->GetPlayerEntities().back();
	Vector2D player_pos = transform_arr_->GetComponent(player_entity->GetID())->GetPosition();
	Vector2D grid_scale = partitioning_->ConvertTransformToGridScale(player_pos);
	
	CollisionMapType col_map{};

	GetPartitionedCollisionMap(static_cast<size_t>(grid_scale.x), static_cast<size_t>(grid_scale.y), col_map);

	if (col_map.empty() || col_map.size() < 2)
		return false;

	//only check player vs burrow
	//player layer == 3
	//hole layer == 4

	if (col_map.find(CollisionLayer::PLAYER) != col_map.end() &&
		col_map.find(CollisionLayer::HOLE) != col_map.end()) {
	
		AABBIt player = col_map[CollisionLayer::PLAYER].find(player_entity->GetID());

		AABBType* hole_map = &col_map[CollisionLayer::HOLE];

		for (AABBIt hole = hole_map->begin(); hole != hole_map->end(); ++hole) {

			float t{};
			if (CheckCollision(*hole->second, Vec2{}, *player->second, Vec2{}, PE_FrameRate.GetFixedDelta(), t)) {

				return true;
			}
		}
	}

	return false;
}

void Collision::ToggleClickables(size_t group) {

	for (ClickableIt clickable = clickable_arr_->begin(); clickable != clickable_arr_->end(); ++clickable) {

		if (clickable->second->group_ == group)
			clickable->second->active_ = !clickable->second->active_;
	}
}

/***************************************************************************/
/*                            Class Functions                              */
/***************************************************************************/

// Init function called to initialise a system
void Collision::Init() {

	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	graphics_ = &*CORE->GetSystem<GraphicsSystem>();
	camera_ = &*CORE->GetSystem<CameraSystem>();
	windows_ = &*CORE->GetSystem<WindowsSystem>();
	partitioning_ = &*CORE->GetSystem<PartitioningSystem>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();

	clickable_arr_ = component_mgr_->GetComponentArray<Clickable>();
	motion_arr_ = component_mgr_->GetComponentArray<Motion>();
	status_arr_ = component_mgr_->GetComponentArray<Status>();
	transform_arr_ = component_mgr_->GetComponentArray<Transform>();
	camera_arr_ = component_mgr_->GetComponentArray<Camera>();
	input_controller_arr_ = component_mgr_->GetComponentArray<InputController>();

	// Defining collision map layering
	/*
		Parameter 1: Collision layer 0
		Parameter 2: Collidable with nothing, does not collide with similar layer
	*/
	AddCollisionLayers(CollisionLayer::BACKGROUND, "00000000", false);

	/*
		Parameter 1: Collision layer 1
		Parameter 2: Collidable with Layer 2 (ENEMY) and Layer 3 (PLAYER),
					 does not collide with similar layer
	*/
	AddCollisionLayers(CollisionLayer::TILES, "00000010", false);

	/*
		Parameter 1: Collision layer 2
		Parameter 2: Collidable with Layer 2 (ENEMY) and Layer 3 (PLAYER),
					 does not collide with similar layer
	*/
	AddCollisionLayers(CollisionLayer::ENEMY, "00000110", false);

	/*
		Parameter 1: Collision layer 3
		Parameter 2: Collidable with Layer 2 (ENEMY) and Layer 3 (PLAYER)
	*/
	AddCollisionLayers(CollisionLayer::PLAYER, "00001110");

	/*
		Parameter 1: Collision layer 4
		Parameter 2: Collidable with Layer 3 (PLAYER)
		"THIS HOLE WAS MADE FOR ME"
	*/
	AddCollisionLayers(CollisionLayer::HOLE, "00001000", false);

	/*
		Parameter 1: Collision layer 5
		Parameter 2: Collidable with Layer 3 (PLAYER)
	*/
	AddCollisionLayers(CollisionLayer::GOAL, "00001000", false);

	/*
		Parameter 1: Collision layer 6
		Parameter 2: Collidable with nothing, does not collide with similar layer
	*/
	AddCollisionLayers(CollisionLayer::UI_ELEMENTS, "00000000", false);

	/*
	Parameter 1: Collision layer 5
	Parameter 2: Collidable with Layer 3 (PLAYER)
	*/
	AddCollisionLayers(CollisionLayer::KEYS, "00001000", false);

	/*
	Parameter 1: Collision layer 5
	Parameter 2: Collidable with Layer 3 (PLAYER)
	*/
	AddCollisionLayers(CollisionLayer::COLLECTIBLE, "00001000", false);

	/*
	Parameter 1: Collision layer 5
	Parameter 2: Collidable with Layer 3 (PLAYER)
	*/
	AddCollisionLayers(CollisionLayer::GATE, "00001000", false);

	M_DEBUG->WriteDebugMessage("Collision System Init\n");
}

void Collision::GetPartitionedCollisionMap(size_t x, size_t y, CollisionMapType& col_map) {
	
	std::vector<AABBIt> vec{};
	partitioning_->GetPartitionedEntities(vec, x, y);
	
	for (std::vector<AABBIt>::iterator it = vec.begin(); it != vec.end(); ++it) {

		CollisionLayer map_layer = static_cast<CollisionLayer>((*it)->second->GetLayer());

		col_map[map_layer].insert({ static_cast<EntityID>((*it)->first), (*it)->second });
	}
}

void Collision::ProcessPartitionedEntities(size_t y, size_t x, float frametime) {										//test fn

	CollisionMapType col_map{};

	GetPartitionedCollisionMap(x, y, col_map);

	if (col_map.empty())
		return;

	for (CollisionMapIt layer1 = col_map.begin(); layer1 != col_map.end(); ++layer1) {
		for (CollisionMapIt layer2 = col_map.begin(); layer2 != col_map.end(); ++layer2) {

			CollisionLayerIt layer1_mask = collision_layer_arr_.find(layer1->first);
			CollisionLayerIt layer2_mask = collision_layer_arr_.find(layer2->first);

			//if same & not meant to collide
			if (!layer1_mask->second.second && (layer1->first == layer2->first))
				continue;

			// check if bit for layer 1 is active, meaning that both layers will interact
			if ((layer1_mask->second.first & layer2_mask->second.first).test(static_cast<size_t>(layer1->first))) {

				//proceed to pass onto next function
				ProcessCollision(layer1, layer2, frametime);
			}
		}
	}
}

// Update function that contains collision checking logic to determine collision
// between entities
void Collision::Update(float frametime) {
	if (debug_) { M_DEBUG->WriteDebugMessage("\nCollision System Update Debug Log:\n"); }

	// Update bounding boxes
	UpdateBoundingBox();
	UpdateClickableBB();


	std::pair<size_t, size_t> sizes = partitioning_->GetAxisSizes();
	std::vector<std::future<void>> futures{};

	for (size_t i = 0; i < sizes.second; ++i) { // y-axis
		for (size_t j = 0; j < sizes.first; ++j) { // x-axis
			
			// If number of entities within the partition <= 1 return
			if (!partitioning_->VerifyPartition(j, i))
				continue;

			// Else perform collision checks on partition
			if (entity_mgr_->GetEntities().size() > 10) {

				futures.push_back(std::async([this, i, j, frametime] { this->ProcessPartitionedEntities(i, j, frametime); }));
			}
			else {

				ProcessPartitionedEntities(i, j, frametime);
			}
		}
	}
	
	for (std::future<void>& future : futures) {

		future.get();
	}

	// Possibly to toggle off debug mode
	if (debug_)
	{

	}
}


// Draw function that renders AABB boxes when debug mode is active
// Color of AABB boxes change on collision to red
void Collision::Draw() {

	if (debug_)
	{
		const float scale = CORE->GetGlobalScale();
		std::vector<std::pair<glm::vec2, glm::vec2>> collided_points;
		std::vector<std::pair<glm::vec2, glm::vec2>> non_collided_points;

		for (CollisionMapIt it = collision_map_.begin(); it != collision_map_.end(); ++it) {
			for (AABBIt aabb = it->second.begin(); aabb != it->second.end(); ++aabb) {

				Vector2D top_right = scale * (*aabb).second->top_right_;
				Vector2D bottom_left = scale * (*aabb).second->bottom_left_;
				

				if ((*aabb).second->collided)
				{
					collided_points.push_back({ {bottom_left.x, bottom_left.y}, {bottom_left.x, top_right.y} });
					collided_points.push_back({ {bottom_left.x, top_right.y}, {top_right.x, top_right.y} });
					collided_points.push_back({ {top_right.x, top_right.y}, {top_right.x, bottom_left.y} });
					collided_points.push_back({ {top_right.x, bottom_left.y}, {bottom_left.x, bottom_left.y} });

					if (collided_points.size() == graphics_->GetBatchSize())
					{
						graphics_->DrawDebugLines(collided_points, { 1.0f, 0.0f, 0.0f, 1.0f }, 2.5f);
						collided_points.clear();
					}
				}

				else
				{
					non_collided_points.push_back({ {bottom_left.x, bottom_left.y}, {bottom_left.x, top_right.y} });
					non_collided_points.push_back({ {bottom_left.x, top_right.y}, {top_right.x, top_right.y} });
					non_collided_points.push_back({ {top_right.x, top_right.y}, {top_right.x, bottom_left.y} });
					non_collided_points.push_back({ {top_right.x, bottom_left.y}, {bottom_left.x, bottom_left.y} });

					if (non_collided_points.size() == graphics_->GetBatchSize())
					{
						graphics_->DrawDebugLines(non_collided_points, { 0.0f, 1.0f, 0.0f, 1.0f }, 2.5f);
						non_collided_points.clear();
					}
				}
			}
		}

		if (collided_points.size() > 0)
		{
			graphics_->DrawDebugLines(collided_points, { 1.0f, 0.0f, 0.0f, 1.0f }, 2.5f);
		}

		if (non_collided_points.size() > 0)
		{
			graphics_->DrawDebugLines(non_collided_points, { 0.0f, 1.0f, 0.0f, 1.0f }, 2.5f);
		}
	}

	//if (debug_) { debug_ = !debug_; }
}

//function more akin to "What to do when message is received" for internal logic
void Collision::SendMessageD(Message* m) {

	switch (m->message_id_) {

	case MessageIDTypes::DEBUG_ALL:
	{
		debug_ = !debug_;
		break;
	}
	case MessageIDTypes::M_MOUSE_PRESS:
	{
		CheckClickableCollision();

		// If in debug mode, allow for selecting of entities
		//if (debug_) {
		//	std::pair<Entity*, std::vector<ComponentTypes>> fake_pair = GetAttachedComponentIDs();
		//}
		break;
	}
	default:
	{
		break;
	}
	}
}