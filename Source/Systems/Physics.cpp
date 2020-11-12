#include "Systems/Physics.h"
#include "Engine/Core.h"
#include <iostream>
#include "Systems/Debug.h"
#include "Components/Status.h"
#include <assert.h>

bool VerifyZeroFloat(const float& val) {

	if (val > -0.001f && val < 0.001f)
		return true;
	return false;
}

auto SnapZero = [](Vector2D& vec) {

	if (VerifyZeroFloat(vec.x)) {

		vec.x = 0.0f;
	}

	if (VerifyZeroFloat(vec.y)) {

		vec.y = 0.0f;
	}
};

Physics::Physics() {

	debug_ = false;
}

void Physics::Init() {
	// Temporary addition for debugging

	M_DEBUG->WriteDebugMessage("Physics System Init\n");

	ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	motion_arr_ = comp_mgr->GetComponentArray<Motion>();
	status_arr_ = comp_mgr->GetComponentArray<Status>();
	transform_arr_ = comp_mgr->GetComponentArray<Transform>();
	force_mgr = &*CORE->GetManager<ForcesManager>();
	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	graphics_sys_ = &*CORE->GetSystem<GraphicsSystem>();
}

void Physics::Update(float frametime) {

	if (CORE->GetCorePauseStatus())
		return;
	if (debug_) { M_DEBUG->WriteDebugMessage("\nPhysics System Update Debug Log:\n"); }
		
	force_mgr->Update(frametime);

	// Updating entity's velocity
	for (MotionIt motion = motion_arr_->begin(); motion != motion_arr_->end(); ++motion) {

		// Perform update of entity's motion component
		//motion->second->velocity_ += motion->second->acceleration_ * frametime;
		motion->second->acceleration_ = force_mgr->GetForce(motion->second->GetOwner()->GetID()) / motion->second->mass_;
		motion->second->velocity_ += motion->second->acceleration_ * frametime;
		motion->second->velocity_ *= 0.80f;

		// If velocity is close to 0, reset to 0	
		SnapZero(motion->second->velocity_);

		// Handles the updating of textures for player and enemy sprites
		TextureHandler(motion);

		// Check whether the entity owns a transform component by checking entity ID
		//TransformIt xform = transform_arr_.find(motion->first);
		Transform* xform = transform_arr_->GetComponent(motion->first);

		if (xform) {
			if (debug_) {
				// Log id of entity and it's updated components that are being updated
				std::stringstream ss;
				ss << "Updating entity: " << std::to_string(motion->first) << "\n";
				ss << "\tCurrent Position: " << xform->position_.x << ", " << xform->position_.y << "\n";
				M_DEBUG->WriteDebugMessage(ss.str());
			}

			// Perform update of entity's transform component
			xform->position_ += motion->second->velocity_ * frametime;

			if (debug_) {
				// Log id of entity and it's updated components that are being updated
				std::stringstream ss;
				ss << "\tUpdated Position: " << xform->position_.x << ", " << xform->position_.y << "\n";
				M_DEBUG->WriteDebugMessage(ss.str());
			}
		}
	}
}

void Physics::TextureHandler(MotionIt motion) {

	// If velocity x is 0 and status is NONE, swap to idle
	AnimationRenderer* renderer = component_mgr_->GetComponent<AnimationRenderer>(motion->first);

	if (renderer) {

		Status* status = status_arr_->GetComponent(motion->first);

		if (status && (status->status_ != StatusType::BURROW && status->status_ != StatusType::INVISIBLE)) {

			Name* name = component_mgr_->GetComponent<Name>(motion->first);

			if (name) {

				if (VerifyZeroFloat(motion->second->velocity_.x) && VerifyZeroFloat(motion->second->velocity_.y)) {

					//to verify is name is correct
					if (name->GetName() == "Player") {

						graphics_sys_->ChangeAnimation(renderer, "Player_Idle");
					}
					else if (name->GetName() == "Enemy" || name->GetName() == "MovingWall" || name->GetName() == "AITest") {

						// Renzo probably needs to add an extra check here to disable this setting if AI detects player
						graphics_sys_->ChangeAnimation(renderer, "Stagbeetle_Idle");
					}
				}
				else {

					if (name->GetName() == "Player") {

						graphics_sys_->ChangeAnimation(renderer, "Player_Walk");

					}
					else if (name->GetName() == "Enemy" || name->GetName() == "MovingWall" || name->GetName() == "AITest") {

						// Renzo probably needs to add an extra check here to disable this setting if AI detects player
						graphics_sys_->ChangeAnimation(renderer, "Stagbeetle_Walk");
					}
				}
			}
		}
	
		if (motion->second->velocity_.x > 0 && motion->second->is_left_) {

			graphics_sys_->FlipTextureY(renderer);
			motion->second->is_left_ = false;
		}
		else if (motion->second->velocity_.x < 0 && !motion->second->is_left_) {

			graphics_sys_->FlipTextureY(renderer);
			motion->second->is_left_ = true;
		}
	}
}

void Physics::ChangeVelocity(Message* m) {
	// If there are multiple players the results will be duplicated
	// because there is no specific entity id at the moment

	//dynamic cast from message base class to derived message class
	MessagePhysics_Motion* msg = dynamic_cast<MessagePhysics_Motion*>(m);

	//locate the motion component that contains a matching entityID as in the message
	for (MotionIt motion = motion_arr_->begin(); motion != motion_arr_->end(); ++motion) {

		// Check if entity type of owner is Player
		if (ENTITYNAME(motion->second->GetOwner()) == "Player") {

			//StatusIt status = status_arr_.find(motion->second->GetOwner()->GetID());
			Status* status = status_arr_->GetComponent(motion->second->GetOwner()->GetID());

			// Temporary inclusion for "Hiding and burrow" check until input sys conversion to component
			if (status && (status->status_ != StatusType::INVISIBLE)) {

				//update the acceleration data member of that component with the message's
				motion->second->velocity_ = msg->new_vec_;

				//std::cout << "New Acceleration: " << motion->second->acceleration_.x << ", " << motion->second->acceleration_.y << std::endl;
			}
			else {
				motion->second->velocity_ = {};
			}
		}
	}
}

void Physics::SendMessageD(Message* msg) {
	switch (msg->message_id_)
	{
		case MessageIDTypes::PHY_UPDATE_VEL:
		{
			//std::cout << "Physics System: Updating velocity of player" << std::endl;
			ChangeVelocity(msg);
			break;
		}
		case MessageIDTypes::DEBUG_ALL:
        {
            debug_ = !debug_;
			break;
        }
		default:
        {
            break;
        }
	}
}