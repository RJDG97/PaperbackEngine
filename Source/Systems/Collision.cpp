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

	return Vector2D{};
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

	//assume that is button
	//compute if position is within bounding box
	if (button->bottom_left_.x <= cursor_pos.x &&
		button->bottom_left_.y <= cursor_pos.y &&
		button->top_right_.x >= cursor_pos.x &&
		button->top_right_.y >= cursor_pos.y) {
		return true;
	}
	return false;
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

	for (ClickableIt clickable = clickable_arr_.begin(); clickable != clickable_arr_.end(); ++clickable) {

		if (CheckCursorCollision(cursor_pos, clickable->second)) {

			Message_Button msg{clickable->second->index_};
			CORE->BroadcastMessage(&msg);
			return;
		}
	}
}

//init function called to initialise a system
void Collision::Init() {

	shdr_pgm_ = *CORE->GetManager<ShaderManager>()->GetShdrpgm("DebugShader");
	model_ = *CORE->GetManager<ModelManager>()->GetModel("LinesModel");
	world_to_ndc_xform_ = &(CORE->GetSystem<GraphicsSystem>()->world_to_ndc_xform_);
	glLineWidth(2.0f);

	M_DEBUG->WriteDebugMessage("Collision System Init\n");
}

void Collision::CollisionWall(AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2, float frametime, float t_first) {
	
	//std::shared_ptr<ForcesManager> force_mgr = CORE->GetManager<ForcesManager>();

	Vector2D inverse_vector_1 = (-(*vel1)) * (frametime - t_first);
	Vector2D inverse_vector_2 = (-(*vel2)) * (frametime - t_first);

	TransformIt transform1 = transform_arr_.find(aabb1->second->GetOwner()->GetID());
	TransformIt transform2 = transform_arr_.find(aabb2->second->GetOwner()->GetID());

	// Get "fake normal" to colliding side
	Vector2D normal = Normal(aabb1->second->scale_, aabb2->second->scale_, transform1->second, transform2->second);

	// Isolate relavent vector value based on normal value
	inverse_vector_1.x *= normal.x;
	inverse_vector_1.y *= normal.y;
	inverse_vector_2.x *= normal.x;
	inverse_vector_2.y *= normal.y;

	transform1->second->position_ += inverse_vector_1;
	transform2->second->position_ += inverse_vector_2;
	

	/*
	MotionIt motion1 = motion_arr_.find(aabb1->second->GetOwner()->GetID());
	MotionIt motion2 = motion_arr_.find(aabb2->second->GetOwner()->GetID());
	if (motion1 != motion_arr_.end()) {
		motion1->second->velocity_ += inverse_vector_1;
	}
	if (motion2 != motion_arr_.end()) {
		motion2->second->velocity_ += inverse_vector_2;
	}
	*/

	/*
	Vector2D accel_1 = inverse_vector_1 / frametime;
	Vector2D accel_2 = inverse_vector_2 / frametime;

	std::shared_ptr<Motion> motion_1 =
		std::dynamic_pointer_cast<Motion>(aabb1->second->GetOwner()->GetComponent(ComponentTypes::MOTION));
	std::shared_ptr<Motion> motion_2 =
		std::dynamic_pointer_cast<Motion>(aabb2->second->GetOwner()->GetComponent(ComponentTypes::MOTION));

	float mass_1 = motion_1 == nullptr ? 0 : motion_1->mass_;
	float mass_2 = motion_2 == nullptr ? 0 : motion_2->mass_;

	Vector2D force_1 = mass_1 * accel_1 * 150.0f;
	Vector2D force_2 = mass_2 * accel_2 * 150.0f;
	//std::cout << "Force 1: " << force_1.x << ", " << force_1.y << std::endl;
	//std::cout << "Force 2: " << force_2.x << ", " << force_2.y << std::endl;
	

	force_mgr->AddForce(aabb1->second->GetOwner()->GetID(), "collision_response", frametime, force_1);
	force_mgr->AddForce(aabb2->second->GetOwner()->GetID(), "collision_response", frametime, force_2);
	*/

	if (debug_) {
		std::string debug_str{};
		debug_str += "time to collision: " + std::to_string(t_first) + "\n"
			+ "Inverse vector 1: " + std::to_string(inverse_vector_1.x) + ", "
			+ std::to_string(inverse_vector_1.y) + "\n";
		M_DEBUG->WriteDebugMessage(debug_str);
	}
}

//contains logic executed during the update loop of a game
void Collision::Update(float frametime) {
	if (debug_) { M_DEBUG->WriteDebugMessage("\nCollision System Update Debug Log:\n"); }

	UpdateBoundingBox();

	UpdateClickableBB();

	Vector2D empty{};

	for (AABBIt aabb1 = aabb_arr_.begin(); aabb1 != aabb_arr_.end(); ++aabb1) {

		AABBIt aabb2 = aabb1;
		MotionIt motion1 = motion_arr_.find(aabb1->second->GetOwner()->GetID());
		//assert(motion1 != nullptr && "aabb1 does not have a motion component");

		Vector2D* vel1{};

		vel1 = (motion1 != motion_arr_.end()) ? &motion1->second->velocity_ : &empty;

		++aabb2;
		
		for (; aabb2 != aabb_arr_.end(); ++aabb2) {

			MotionIt motion2 = motion_arr_.find(aabb2->second->GetOwner()->GetID());
			//assert(motion2 != nullptr && "aabb2 does not have a motion component");

			Vector2D* vel2{};

			vel2 = (motion2 != motion_arr_.end()) ? &motion2->second->velocity_ : &empty;

			float t_first{};

			if (CheckCollision(*aabb1->second, *vel1, *aabb2->second, *vel2, frametime, t_first)) {

				std::string aabb1_type = ENTITYNAME(aabb1->second->GetOwner());
				std::string aabb2_type = ENTITYNAME(aabb2->second->GetOwner());

				//check what types are both objects that are colliding
				if ((aabb1_type == "Wall" && (aabb2_type == "Player" || aabb2_type == "Enemy")) ||
					(aabb2_type == "Wall" && (aabb1_type == "Player" || aabb1_type == "Enemy"))) {

					// Toggle true to trigger bounding box color
					aabb1->second->collided = true;
					aabb2->second->collided = true;

					// Handles collision response for when moving entity collides with static entity
					CollisionWall(aabb1, vel1, aabb2, vel2, frametime, t_first);
				}
				else {

					//enable to check if collision detected
					//currently disabled to show that burrowing works
					//aabb1->second->collided = true;
					//aabb2->second->collided = true;

					//otherwise colliding are player & enemy or player & player
					if ((aabb1_type == "Player" && aabb2_type == "Enemy") ||
						(aabb1_type == "Enemy" && aabb2_type == "Player")) {

						StatusIt player_status;

						if (aabb1_type == "Player") {

							player_status = status_arr_.find(aabb1->second->GetOwner()->GetID());
						}
						else {

							player_status = status_arr_.find(aabb2->second->GetOwner()->GetID());
						}
						
						if (player_status->second) {

							if (player_status->second->status_ == StatusType::NONE) {

								aabb1->second->collided = true;
								aabb2->second->collided = true;

								player_status->second->status_ = StatusType::HIT;
								player_status->second->status_timer_ = 5.1f;
							}
							
							else if (player_status->second->status_ == StatusType::INVISIBLE) {

							}
							
						}
					}
				}
			}
		}
	}

	// Test it here as well
	//UpdateBoundingBox();
	if (debug_)
	{

	}
}

void Collision::Draw() {

	if (debug_)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (AABBIt aabb = aabb_arr_.begin(); aabb != aabb_arr_.end(); ++aabb)
		{
			Vector2D top_right = (*aabb).second->top_right_;
			Vector2D bottom_left = (*aabb).second->bottom_left_;

			Vector2D aabb_middle = bottom_left + (top_right - bottom_left)/2;

			glm::mat3 scaling = glm::mat3{ (top_right.x - bottom_left.x)/2, 0.0f, 0.0f,
										   0.0f, (top_right.y - bottom_left.y)/2, 0.0f,
										   0.0f, 0.0f, 1.0f };

			glm::mat3 translation{ 1.0f, 0.0f, 0.0f,
								   0.0f, 1.0f, 0.0f,
								   aabb_middle.x, aabb_middle.y, 1.0f };

			glm::mat3 mdl_to_ndc_xform = *(world_to_ndc_xform_) * translation * scaling;

			shdr_pgm_.Use();
			glBindVertexArray(model_.vaoid_);

			shdr_pgm_.SetUniform("uModel_to_NDC", mdl_to_ndc_xform);
			shdr_pgm_.SetUniform("collided", (*aabb).second->collided);

			glDrawArrays(GL_LINES, 0, model_.draw_cnt_);

			// after completing the rendering, we tell the driver that the VAO vaoid
			// and the current shader program are no longer current
			glBindVertexArray(0);

			shdr_pgm_.UnUse();
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
			break;
		}
		default:
		{
			break;
		}
	}
}

void Collision::AddAABBComponent(EntityID id, AABB* aabb) {

	M_DEBUG->WriteDebugMessage("Adding AABB Component to entity: " + std::to_string(id) + "\n");

	aabb_arr_[id] = aabb;
}

void Collision::RemoveAABBComponent(EntityID id) {

	AABBIt it = aabb_arr_.find(id);

	if (it != aabb_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing AABB Component from entity: " + std::to_string(id) + "\n");
		aabb_arr_.erase(it);
	}
}

void Collision::AddClickableComponent(EntityID id, Clickable* clickable) {

	M_DEBUG->WriteDebugMessage("Adding AABB Component to entity: " + std::to_string(id) + "\n");

	clickable_arr_[id] = clickable;
}

void Collision::RemoveClickableComponent(EntityID id) {

	ClickableIt it = clickable_arr_.find(id);

	if (it != clickable_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing AABB Component from entity: " + std::to_string(id) + "\n");
		clickable_arr_.erase(it);
	}
}

void Collision::AddMotionComponent(EntityID id, Motion* motion) {

	M_DEBUG->WriteDebugMessage("Adding Motion Component to entity: " + std::to_string(id) + "\n");

	motion_arr_[id] = motion;
}

void Collision::RemoveMotionComponent(EntityID id) {

	MotionIt it = motion_arr_.find(id);

	if (it != motion_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Motion Component from entity: " + std::to_string(id) + "\n");
		motion_arr_.erase(it);
	}
}

void Collision::AddStatusComponent(EntityID id, Status* status) {

	M_DEBUG->WriteDebugMessage("Adding Status Component to entity: " + std::to_string(id) + "\n");
	status_arr_[id] = status;
}

void Collision::RemoveStatusComponent(EntityID id) {

	StatusIt it = status_arr_.find(id);

	if (it != status_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Status Component from entity: " + std::to_string(id) + "\n");
		status_arr_.erase(it);
	}
}

void Collision::AddTransformComponent(EntityID id, Transform* status) {

	M_DEBUG->WriteDebugMessage("Adding Transform Component to entity: " + std::to_string(id) + "\n");
	transform_arr_[id] = status;
}

void Collision::RemoveTransformComponent(EntityID id) {

	TransformIt it = transform_arr_.find(id);

	if (it != transform_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Transform Component from entity: " + std::to_string(id) + "\n");
		transform_arr_.erase(it);
	}
}

void Collision::AddInputControllerComponent(EntityID id, InputController* input_controller) {

	M_DEBUG->WriteDebugMessage("Adding Input Controller Component to entity: " + std::to_string(id) + "\n");
	input_controller_arr_[id] = input_controller;
}

void Collision::RemoveInputControllerComponent(EntityID id) {

	InputControllerIt it = input_controller_arr_.find(id);

	if (it != input_controller_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Input Controller Component from entity: " + std::to_string(id) + "\n");
		input_controller_arr_.erase(it);
	}
}

void Collision::UpdateBoundingBox() {
	if (debug_)
		M_DEBUG->WriteDebugMessage("Collision System: Updating Bounding Boxes\n");

	for (AABBIt aabb = aabb_arr_.begin(); aabb != aabb_arr_.end(); ++aabb) {
		
		//reset collided flag to false to prepare for collision check after
		aabb->second->collided = false;

		Entity* entity = aabb->second->GetOwner();
		DEBUG_ASSERT((entity), "Entity does not exist");

		TransformIt entity_position = transform_arr_.find(entity->GetID());

		aabb->second->bottom_left_ = entity_position->second->position_ - aabb->second->scale_;
		aabb->second->top_right_ = entity_position->second->position_ + aabb->second->scale_;
	}
}

void Collision::UpdateClickableBB() {
	if (debug_)
		M_DEBUG->WriteDebugMessage("Collision System: Updating Clickable Bounding Boxes\n");

	for (ClickableIt clickable = clickable_arr_.begin(); clickable != clickable_arr_.end(); ++clickable) {

		//reset collided flag to false to prepare for collision check after
		clickable->second->collided_ = false;

		Entity* entity = clickable->second->GetOwner();
		DEBUG_ASSERT((entity), "Entity does not exist");

		TransformIt entity_position = transform_arr_.find(clickable->second->GetOwner()->GetID());

		clickable->second->bottom_left_ = entity_position->second->position_ - clickable->second->scale_;
		clickable->second->top_right_ = entity_position->second->position_ + clickable->second->scale_;
	}
}