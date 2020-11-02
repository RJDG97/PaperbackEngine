#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/Collision.h"
#include "Systems/Factory.h"
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
#include <iostream>
#include <assert.h>
#include <glm/gtc/type_ptr.hpp>

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
Vector2D Normal(const Vector2D& s1, const Vector2D& s2,
	const Transform* t1, const Transform* t2) {

	Vector2D scale_sum = s1 + s2;
	Vector2D vec = absVec(t1->GetPosition() - t2->GetPosition());

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

	if (SeparatingAxisTheorem(aabb1, aabb2))
	{
		Vector2D Vb;
		tFirst = 0;

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
				tFirst = max(abs(aab1_top_right.x - aab2_bot_left.x) / Vb.x, tFirst);
			if (aab1_bot_left.x < aab2_top_right.x)
				tLast = min(abs(aab1_bot_left.x - aab2_top_right.x) / Vb.x, tLast);
		}
		if (Vb.x > EPSILON)
		{
			//case 2
			if (aab1_bot_left.x > aab2_top_right.x)
				tFirst = max(abs(aab1_bot_left.x - aab2_top_right.x) / Vb.x, tFirst);
			if (aab1_top_right.x > aab2_bot_left.x)
				tLast = min(abs(aab1_top_right.x - aab2_bot_left.x) / Vb.x, tLast);
			//case3
			if (aab1_top_right.x < aab2_bot_left.x)
				return 0;
		}
		if (tFirst > tLast)
			return 0;

		tFirst = 0, tLast = dt;

		// Y-Axis check
		if (Vb.y < -EPSILON)
		{
			//case 1
			if (aab1_bot_left.y > aab2_top_right.y)
				return 0;
			//case 4
			if (aab1_top_right.y < aab2_bot_left.y)
				tFirst = max(abs(aab1_top_right.y - aab2_bot_left.y) / Vb.y, tFirst);
			if (aab1_bot_left.y < aab2_top_right.y)
				tLast = min(abs(aab1_bot_left.y - aab2_top_right.y) / Vb.y, tLast);
		}
		if (Vb.y > EPSILON)
		{
			//case 2
			if (aab1_bot_left.y > aab2_top_right.y)
				tFirst = max(abs(aab1_bot_left.y - aab2_top_right.y) / Vb.y, tFirst);
			if (aab1_top_right.y > aab2_bot_left.y)
				tLast = min(abs(aab1_top_right.y - aab2_bot_left.y) / Vb.y, tLast);
			//case3
			if (aab1_top_right.y < aab2_bot_left.y)
				return 0;
		}
		if (tFirst > tLast)
			return 0;
	}
	return 1;
}

bool Collision::CheckCursorCollision(const Vec2& cursor_pos, const Clickable* button) {

	Vec2 cursor_pos_scaled = (1 / *cam_zoom_) * cursor_pos;

	//assume that is button
	//compute if position is within bounding box
	if (button->bottom_left_.x <= cursor_pos_scaled.x &&
		button->bottom_left_.y <= cursor_pos_scaled.y &&
		button->top_right_.x >= cursor_pos_scaled.x &&
		button->top_right_.y >= cursor_pos_scaled.y) {
		return true;
	}
	return false;
}

bool Collision::CheckCursorCollision(const Vec2& cursor_pos, const AABB* box) {

	Vec2 cursor_pos_scaled = (1 / *cam_zoom_) * cursor_pos;

	//assume that is button
	//compute if position is within bounding box
	if (box->bottom_left_.x <= cursor_pos_scaled.x &&
		box->bottom_left_.y <= cursor_pos_scaled.y &&
		box->top_right_.x >= cursor_pos_scaled.x &&
		box->top_right_.y >= cursor_pos_scaled.y) {
		return true;
	}
	return false;
}

std::pair<Entity*, std::vector<ComponentTypes>> Collision::GetAttachedComponentIDs() {
	std::vector<ComponentTypes> comp_arr;
	Entity* entity;
	EntityID selected_entity_id = SelectEntity();

	if (selected_entity_id != 0) {
		// Grab entity from factory and return the bitset
		std::shared_ptr<EntityFactory> factory = CORE->GetSystem<EntityFactory>();
		entity = factory->GetObjectWithID(selected_entity_id);

		// If entity exists in factory
		if (entity) {
			// Grab component array from entity
			ComponentArr arr = entity->GetComponentArr();
			// Resize component array accordingly
			comp_arr.reserve(arr.size());

			// Iterate all components and save the enums
			for (ComponentArrIt it = arr.begin(); it != arr.end(); ++it) {
				comp_arr.push_back((*it)->GetComponentTypeID());
			}
		}
	}

	// Return compiled entity 
	return std::make_pair(entity, comp_arr);
}

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

		if (CheckCursorCollision(cursor_pos, clickable->second)) {

			Message_Button msg{ clickable->second->index_ };
			CORE->BroadcastMessage(&msg);
			return;
		}
	}
}

EntityID Collision::SelectEntity() {

	Vector2D cursor_pos = CORE->GetSystem<InputSystem>()->GetCursorPosition();

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

	Vector2D inverse_vector_1 = (-(*vel1)) * (frametime - t_first);
	Vector2D inverse_vector_2 = (-(*vel2)) * (frametime - t_first);

	Transform* transform1 = transform_arr_->GetComponent(aabb1->second->GetOwner()->GetID());
	Transform* transform2 = transform_arr_->GetComponent(aabb2->second->GetOwner()->GetID());

	// Get "fake normal" to colliding side
	Vector2D normal = Normal(aabb1->second->scale_, aabb2->second->scale_, transform1, transform2);

	// Isolate relavent vector value based on normal value
	inverse_vector_1.x *= normal.x;
	inverse_vector_1.y *= normal.y;
	inverse_vector_2.x *= normal.x;
	inverse_vector_2.y *= normal.y;

	// Reposition entity's position
	transform1->position_ += inverse_vector_1;
	transform2->position_ += inverse_vector_2;

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

bool Collision::PlayervEnemyResponse(AABBIt aabb1, AABBIt aabb2) {
	Status* player_status = status_arr_->GetComponent(aabb2->second->GetOwner()->GetID());;

	if (player_status) {

		if (player_status->status_ == StatusType::NONE) {

			// Set collided status
			aabb1->second->collided = true;
			aabb2->second->collided = true;

			player_status->status_ = StatusType::HIT;
			player_status->status_timer_ = 2.0f;
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

void Collision::CollisionResponse(const CollisionLayer& layer_a, const CollisionLayer& layer_b,
	AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2,
	float frametime, float t_first) {

	switch (layer_a)
	{
	case CollisionLayer::TILES:
	{
		DefaultResponse(aabb1, vel1, aabb2, vel2, frametime, t_first);
		break;
	}
	case CollisionLayer::ENEMY:
	{
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
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

void Collision::ProcessCollision(CollisionLayerIt col_layer_a, CollisionLayerIt col_layer_b, float frametime) {

	CollidableLayer mask_a{};
	mask_a.set(static_cast<size_t>(col_layer_a->first));

	//get collision flag value
	if (((col_layer_a->second.first & col_layer_b->second.first) & mask_a) == mask_a) {

		CollisionLayer layer_a = static_cast<CollisionLayer>(col_layer_a->first);
		CollisionLayer layer_b = static_cast<CollisionLayer>(col_layer_b->first);

		for (AABBIt layer_a_it = collision_map_[layer_a].begin(); layer_a_it != collision_map_[layer_a].end(); ++layer_a_it) {
			for (AABBIt layer_b_it = collision_map_[layer_b].begin(); layer_b_it != collision_map_[layer_b].end(); ++layer_b_it) {

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

			aabb->second->bottom_left_ = entity_position->position_ - aabb->second->scale_;
			aabb->second->top_right_ = entity_position->position_ + aabb->second->scale_;
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


/***************************************************************************/
/*                            Class Functions                              */
/***************************************************************************/

// Init function called to initialise a system
void Collision::Init() {

	ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	clickable_arr_ = comp_mgr->GetComponentArray<Clickable>();
	motion_arr_ = comp_mgr->GetComponentArray<Motion>();
	status_arr_ = comp_mgr->GetComponentArray<Status>();
	transform_arr_ = comp_mgr->GetComponentArray<Transform>();
	input_controller_arr_ = comp_mgr->GetComponentArray<InputController>();

	shdr_pgm_ = CORE->GetManager<ShaderManager>()->AddShdrpgm("Shaders/debug.vert", "Shaders/debug.frag", "DebugShader");
	model_ = CORE->GetManager<ModelManager>()->AddLinesModel(1, 1, "LinesModel");
	world_to_ndc_xform_ = &(CORE->GetSystem<CameraSystem>()->world_to_ndc_xform_);
	cam_zoom_ = &(CORE->GetSystem<CameraSystem>()->cam_zoom_);
	glLineWidth(2.0f);

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
	AddCollisionLayers(CollisionLayer::PLAYER, "00000110");
	/*
		Parameter 1: Collision layer 4
		Parameter 2: Collidable with nothing, does not collide with similar layer
	*/
	AddCollisionLayers(CollisionLayer::UI_ELEMENTS, "00000000", false);

	M_DEBUG->WriteDebugMessage("Collision System Init\n");
}

// Update function that contains collision checking logic to determine collision
// between entities
void Collision::Update(float frametime) {
	if (debug_) { M_DEBUG->WriteDebugMessage("\nCollision System Update Debug Log:\n"); }

	// Update bounding boxes
	UpdateBoundingBox();
	UpdateClickableBB();

	Vector2D empty{};

	for (CollisionLayerIt it1 = collision_layer_arr_.begin(); it1 != collision_layer_arr_.end(); ++it1) {
		for (CollisionLayerIt it2 = collision_layer_arr_.begin(); it2 != collision_layer_arr_.end(); ++it2) {

			if (!it1->second.second && (it1->second == it2->second))
				continue;

			ProcessCollision(it1, it2, frametime);
		}
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (CollisionMapIt it = collision_map_.begin(); it != collision_map_.end(); ++it) {
			for (AABBIt aabb = it->second.begin(); aabb != it->second.end(); ++aabb) {

				Vector2D top_right = (*aabb).second->top_right_;
				Vector2D bottom_left = (*aabb).second->bottom_left_;

				Vector2D aabb_middle = bottom_left + (top_right - bottom_left) / 2;

				glm::mat3 scaling = glm::mat3{ (top_right.x - bottom_left.x) / 2, 0.0f, 0.0f,
											   0.0f, (top_right.y - bottom_left.y) / 2, 0.0f,
											   0.0f, 0.0f, 1.0f };

				glm::mat3 translation{ 1.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f,
									   aabb_middle.x, aabb_middle.y, 1.0f };

				glm::mat3 mdl_to_ndc_xform = *(world_to_ndc_xform_)*translation * scaling;

				shdr_pgm_->Use();
				glBindVertexArray(model_->vaoid_);

				shdr_pgm_->SetUniform("uModel_to_NDC", mdl_to_ndc_xform);
				shdr_pgm_->SetUniform("collided", (*aabb).second->collided);

				glDrawArrays(GL_LINES, 0, model_->draw_cnt_);

				// after completing the rendering, we tell the driver that the VAO vaoid
				// and the current shader program are no longer current
				glBindVertexArray(0);

				shdr_pgm_->UnUse();
			}
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
		if (debug_) {
			std::pair<Entity*, std::vector<ComponentTypes>> fake_pair = GetAttachedComponentIDs();
		}
		break;
	}
	default:
	{
		break;
	}
	}
}