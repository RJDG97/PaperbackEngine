#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/Collision.h"
#include "Systems/Factory.h"
#include "Systems/Debug.h"
#include "Components/Scale.h"
#include "Components/Transform.h"
#include "Entity/ComponentCreator.h"
#include "Entity/ComponentTypes.h"
#include "Components/AABB.h"
#include "Components/Motion.h"
#include "Components/Status.h"
#include <iostream>
#include <assert.h>
#include <glm/gtc/type_ptr.hpp>

#define EPSILON 0.001f

Collision* COLLISION;

Collision::Collision() {
	
	debug_ = true;
}

// Comparison function
auto max = [](float a, float b) {
	return (a > b) ? a : b;
};

auto min = [](float a, float b) {
	return (a < b) ? a : b;
};

bool Collision::CheckCollision(const AABB& aabb1, const Vec2& vel1,
							   const AABB& aabb2, const Vec2& vel2,
							   const float dt, float& tFirst) {

	// AABB_1
	Vector2D aab1_bot_left = aabb1.GetBottomLeft();
	Vector2D aab1_top_right = aabb1.GetTopRight();

	// AABB_2
	Vector2D aab2_bot_left = aabb2.GetBottomLeft();
	Vector2D aab2_top_right = aabb2.GetTopRight();

	if ((aab1_bot_left.x > aab2_top_right.x || aab1_top_right.x < aab2_bot_left.x) ||
		(aab1_bot_left.y > aab2_top_right.y || aab1_top_right.y < aab2_bot_left.y))
	{
		
		Vector2D Vb;
		tFirst = 0;
		float tLast = dt; // g_dt does not exist yet
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
	
	/*
	if (((aab1_bot_left.x <= aab2_top_right.x && aab1_bot_left.x >= aab2_bot_left.x) ||
		(aab1_top_right.x >= aab2_bot_left.x && aab1_top_right.x <= aab2_top_right.x)) &&
		((aab1_bot_left.y >= aab2_bot_left.y && aab1_bot_left.y <= aab2_top_right.y) ||
		 (aab1_top_right.y >= aab2_bot_left.y && aab1_top_right.y <= aab2_top_right.y))) {
		return 1;
	}
	return 0;
	*/
}

//init function called to initialise a system
void Collision::Init() {

	FACTORY->AddComponentCreator("AABB", new ComponentCreatorType<AABB>(ComponentTypes::AABB));

	shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm("DebugShader");
	model_ = CORE->GetManager<ModelManager>()->GetModel("LinesModel");
	world_to_ndc_xform_ = &(CORE->GetSystem<GraphicsSystem>()->world_to_ndc_xform_);
	glLineWidth(2.0f);

	M_DEBUG->WriteDebugMessage("Collision System Init\n");
}

void Collision::CollisionWall(AABBIt aabb1, Vec2* vel1, AABBIt aabb2, Vec2* vel2, float frametime, float t_first) {

	/*
				std::cout << "Collision detected between " << aabb1->second->GetOwner()->GetID()
					<< " and " << aabb2->second->GetOwner()->GetID() << std::endl;
					*/
					/*std::cout << "AABB1 bottom left: " << aabb1->second->bottom_left_.x << ", " << aabb1->second->bottom_left_.y
						<< " | AABB1 top right: " << aabb1->second->top_right_.x << ", " << aabb1->second->top_right_.y << std::endl;

					std::cout
					<< "AABB2 bottom left: " << aabb2->second->bottom_left_.x
					<< ", " << aabb2->second->bottom_left_.y
					<< " | AABB2 top right: " << aabb2->second->top_right_.x
					<< ", " << aabb2->second->top_right_.y
					<< std::endl;*/

					//std::cout << "time to collision: " << t_first << std::endl;
	Vector2D inverse_vector_1 = (-(*vel1)) * (frametime - t_first);
	Vector2D inverse_vector_2 = (-(*vel2)) * (frametime - t_first);

	Transform* transform1 = dynamic_cast<Transform*>(aabb1->second->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));
	Transform* transform2 = dynamic_cast<Transform*>(aabb2->second->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));

	//std::cout << "Inverse vector 1: " << inverse_vector_1.x << ", " << inverse_vector_1.y << std::endl;
	//std::cout << "Inverse vector 2: " << inverse_vector_2.x << ", " << inverse_vector_2.y << std::endl;

	transform1->position_ += inverse_vector_1;
	transform2->position_ += inverse_vector_2;

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

	/*for (AABBIt box = aabb_arr_.begin(); box != aabb_arr_.end(); ++ box) {
		
		std::cout << "AABB " << counter << ": "
				  << "Top left: " << box->second->bottom_left_.x
				  << ", " << box->second->bottom_left_.y
				  << "\nBottom right: " << box->second->top_right_.x
				  << ", " << box->second->top_right_.y
				  << std::endl;
	}*/

	for (AABBIt aabb1 = aabb_arr_.begin(); aabb1 != aabb_arr_.end(); ++aabb1) {

		AABBIt aabb2 = aabb1;
		Motion* motion1 = dynamic_cast<Motion*>(aabb1->second->GetOwner()->GetComponent(ComponentTypes::MOTION));
		//assert(motion1 != nullptr && "aabb1 does not have a motion component");

		Vec2* vel1{};

		vel1 = (motion1 != nullptr) ? &motion1->velocity_ : &Vec2{};

		++aabb2;
		
		for (; aabb2 != aabb_arr_.end(); ++aabb2) {

			Motion* motion2 = dynamic_cast<Motion*>(aabb2->second->GetOwner()->GetComponent(ComponentTypes::MOTION));
			//assert(motion2 != nullptr && "aabb2 does not have a motion component");

			Vec2* vel2{};

			vel2 = (motion2 != nullptr) ? &motion2->velocity_ : &Vec2{};

			float t_first{};

			if (CheckCollision(*aabb1->second, *vel1, *aabb2->second, *vel2, frametime, t_first)) {

				EntityTypes aabb1_type = aabb1->second->GetOwner()->GetType();
				EntityTypes aabb2_type = aabb2->second->GetOwner()->GetType();

				//check what types are both objects that are colliding
				if ((aabb1_type == EntityTypes::Wall && (aabb2_type == EntityTypes::Player || aabb2_type == EntityTypes::Enemy)) ||
					(aabb2_type == EntityTypes::Wall && (aabb1_type == EntityTypes::Player || aabb1_type == EntityTypes::Enemy))) {

					aabb1->second->collided = true;
					aabb2->second->collided = true;
					// Handles collision response for when moving entity collides with static entity
					CollisionWall(aabb1, vel1, aabb2, vel2, frametime, t_first);
				}
				else {

					//otherwise colliding are player & enemy or player & player
					if ((aabb1_type == EntityTypes::Player && aabb2_type == EntityTypes::Enemy) ||
						(aabb1_type == EntityTypes::Enemy && aabb2_type == EntityTypes::Player)) {

						Status* player_status = nullptr;

						if (aabb1_type == EntityTypes::Player) {

							player_status = dynamic_cast<Status*>(aabb1->second->GetOwner()->GetComponent(ComponentTypes::STATUS));
						}
						else {

							player_status = dynamic_cast<Status*>(aabb2->second->GetOwner()->GetComponent(ComponentTypes::STATUS));
						}
						
						if (player_status) {

							if (player_status->status_ == StatusType::NONE) {

								aabb1->second->collided = true;
								aabb2->second->collided = true;

								player_status->status_ = StatusType::HIT;
								player_status->status_timer_ = 5.1f;
							}
							/*
							else if (player_status->status_ == StatusType::INVISIBLE) {

							}
							*/
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

			GLint uniform_var_transform =
				glGetUniformLocation(shdr_pgm_.GetHandle(), "uModel_to_NDC");

			if (uniform_var_transform >= 0) {
				glUniformMatrix3fv(uniform_var_transform, 1, GL_FALSE,
								   glm::value_ptr(mdl_to_ndc_xform));
			}

			else {
				std::cout << "Uniform variable doesn't exist!!!\n";
				std::exit(EXIT_FAILURE);
			}

			GLint uniform_var_collided =
				glGetUniformLocation(shdr_pgm_.GetHandle(), "collided");

			if (uniform_var_collided >= 0) {
				glUniform1i(uniform_var_collided, (*aabb).second->collided);
			}

			else {
				std::cout << "Uniform variable doesn't exist!!!\n";
				std::exit(EXIT_FAILURE);
			}

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
			debug_ = true;
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

void Collision::UpdateBoundingBox() {
	if (debug_)
		M_DEBUG->WriteDebugMessage("Collision System: Updating Bounding Boxes\n");

	for (AABBIt aabb = aabb_arr_.begin(); aabb != aabb_arr_.end(); ++aabb) {
		
		//reset collided flag to false to prepare for collision check after
		aabb->second->collided = false;

		Entity* entity = aabb->second->GetOwner();
		assert(entity && "Entity does not exist");

		Scale* entity_scale = dynamic_cast<Scale*>(aabb->second->GetOwner()->GetComponent(ComponentTypes::SCALE));
		Transform* entity_position = dynamic_cast<Transform*>(aabb->second->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));

		aabb->second->bottom_left_ = entity_position->position_ - entity_scale->scale_;
		aabb->second->top_right_ = entity_position->position_ + entity_scale->scale_;
	}
}