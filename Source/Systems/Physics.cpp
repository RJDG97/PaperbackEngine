#include "Systems/Physics.h"
#include "Engine/Core.h"
#include <iostream>
#include "Systems/Debug.h"

Physics* PHYSICS;

Physics::Physics() {
	PHYSICS = this;
	debug_ = false;
}

void Physics::Init() {
	// Temporary addition for debugging
	FACTORY->AddComponentCreator("Transform", new ComponentCreatorType<Transform>( ComponentTypes::TRANSFORM));
	//FACTORY->AddComponentCreator("Health", new ComponentCreatorType<Health>(ComponentTypes::HEALTH));
	FACTORY->AddComponentCreator("Motion", new ComponentCreatorType<Motion>(ComponentTypes::MOTION));

	M_DEBUG->WriteDebugMessage("Physics System Init\n");
}

void Physics::Update(float frametime) {
	if (debug_) { M_DEBUG->WriteDebugMessage("\nPhysics System Update Debug Log:\n"); }

	// Updating entity's velocity
	for (MotionIt motion = motion_arr_.begin(); motion != motion_arr_.end(); ++motion) {

		// Perform update of entity's motion component
		motion->second->velocity_ += motion->second->acceleration_ * frametime;

		// Check whether the entity owns a transform component by checking entity ID
		TransformIt xform = transform_arr_.find(motion->first);
		if (xform != transform_arr_.end()) {
			if (debug_) {
				// Log id of entity and it's updated components that are being updated
				std::stringstream ss;
				ss << "Updating entity: " << std::to_string(xform->first) << "\n";
				ss << "\tCurrent Position: " << xform->second->position_.x << ", " << xform->second->position_.y << "\n";
				M_DEBUG->WriteDebugMessage(ss.str());
			}

			// Perform update of entity's transform component
			xform->second->position_ += motion->second->velocity_ * frametime;

			if (debug_) {
				// Log id of entity and it's updated components that are being updated
				std::stringstream ss;
				ss << "\tUpdated Position: " << xform->second->position_.x << ", " << xform->second->position_.y << "\n";
				M_DEBUG->WriteDebugMessage(ss.str());
			}
		}
	}
	if (debug_) { debug_ = !debug_; }
}

void Physics::PublishResults() {

	/*for (PosIt it = Transforms.begin(); it != Transforms.end(); ++it) {
		(it)->PublishResults();
	}*/

	for (TransformIt it = transform_arr_.begin(); it != transform_arr_.end(); ++it) {
		it->second->PublishResults();
	}
}

// Interprets a message that contains an updated acceleration Vec2D
// that will replace the old acceleration vector
// Assumes that the new acceleration vector has been calculated prior to receiving it
/*void Physics::ChangeAcceleration(Message* m) {
	// If there are multiple players the results will be duplicated
	// because there is no specific entity id at the moment

	std::cout << "Entered ChangeAcceleration" << std::endl;
	//dynamic cast from message base class to derived message class
	MessagePhysics_Accel* msg = dynamic_cast<MessagePhysics_Accel*>(m);

	//locate the motion component that contains a matching entityID as in the message
	for (MotionIt motion = motion_arr_.begin(); motion != motion_arr_.end(); ++motion) {

		//std::cout << "Looking for: " << (int)EntityTypes::Player << " vs " << (int)motion->second.GetOwner()->GetType() << std::endl;
		if (motion->second->GetOwner()->GetType() == EntityTypes::Player) {

			//update the acceleration data member of that component with the message's
			motion->second->velocity_ = msg->new_acceleration_;

			//std::cout << "New Acceleration: " << motion->second->acceleration_.x << ", " << motion->second->acceleration_.y << std::endl;
		}
	}
}*/

void Physics::ChangeVelocity(Message* m) {
	// If there are multiple players the results will be duplicated
	// because there is no specific entity id at the moment

	//std::cout << "Entered ChangeVelocity" << std::endl;
	//dynamic cast from message base class to derived message class
	MessagePhysics_Motion* msg = dynamic_cast<MessagePhysics_Motion*>(m);

	//locate the motion component that contains a matching entityID as in the message
	for (MotionIt motion = motion_arr_.begin(); motion != motion_arr_.end(); ++motion) {

		//std::cout << "Looking for: " << (int)EntityTypes::Player << " vs " << (int)motion->second.GetOwner()->GetType() << std::endl;
		if (motion->second->GetOwner()->GetType() == EntityTypes::Player) {

			//update the acceleration data member of that component with the message's
			motion->second->velocity_ = msg->new_vec_;

			//std::cout << "New Acceleration: " << motion->second->acceleration_.x << ", " << motion->second->acceleration_.y << std::endl;
		}
	}
}

void Physics::AddTransformComponent(EntityID id, Transform* transform) {

	M_DEBUG->WriteDebugMessage("Adding Transform Component to entity: " + std::to_string(id) + "\n");
	transform_arr_[id] = transform;
}

void Physics::RemoveTransformComponent(EntityID id) {

	TransformIt it = transform_arr_.find(id);

	if (it != transform_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Transform Component from entity: " + std::to_string(id) + "\n");
		transform_arr_.erase(it);
	}
}

void Physics::AddMotionComponent(EntityID id, Motion* motion) {

	M_DEBUG->WriteDebugMessage("Adding Motion Component to entity: " + std::to_string(id) + "\n");
	motion_arr_[id] = motion;
}

void Physics::RemoveMotionComponent(EntityID id) {

	MotionIt it = motion_arr_.find(id);

	if (it != motion_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Motion Component from entity: " + std::to_string(id) + "\n");
		motion_arr_.erase(it);
	}
}

void Physics::SendMessageD(Message* msg) {
	
	//std::cout << "Message received by Physics" << std::endl;

	/*if (msg->message_id_ == MessageIDTypes::Rotate) {
		std::cout << "prepare to rotate" << std::endl;

		MessageRotation* m = dynamic_cast<MessageRotation*>(msg);
		TransformIt it = PHYSICS->transform_arr_.find(m->entity_one_);

		if (it != PHYSICS->transform_arr_.end()) {
			Rotate((&it->second));
		}
		/*for (PosIt it = PHYSICS->Transforms.begin(); it != PHYSICS->Transforms.end(); ++it) {

			// look into swapping from std::list to std::unordered_map<entityid, component*> to allow for std::find usage
			if (it->GetOwner()->GetID() == m->entityone) {
				Rotate(&*it);
				break;
			}
		}
	}*/

	/*if (msg->message_id_ == MessageIDTypes::HP) {

		std::cout << "prepare to decrement hp" << std::endl;

		MessageHPDecre* m = dynamic_cast<MessageHPDecre*>(msg);
		HPIt it = PHYSICS->hp_arr_.find(m->entity_one_);

		if (it != PHYSICS->hp_arr_.end()) {
			DecreaseHP(&it->second);
		}
	}*/

	switch (msg->message_id_)
	{
		case MessageIDTypes::PHY_UpdateAccel:
		{
			std::cout << "Physics System: Updating acceleration of player" << std::endl;
			//ChangeVelocity(msg);
			//ChangeAcceleration(msg)
			break;
		}
		case MessageIDTypes::PHY_UpdateVel:
		{
			//std::cout << "Physics System: Updating velocity of player" << std::endl;
			ChangeVelocity(msg);
			break;
		}
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


//testing functions for test components
void Physics::Rotate(Transform* pos) {

	pos->rotation_ += 0.2f;

	std::cout << "Message recieved, rotation success, currently at: " 
			  << pos->rotation_
			  << " ; Entity ID: " 
			  << pos->GetOwner()->GetID()
			  << std::endl;
}
//
//void Physics::DecreaseHP(Health* hp) {
//
//	hp->current_health_ -= 1;
//
//	std::cout << "Message recieved, health decremented successfully, currently at: "
//			  << hp->current_health_
//			  << " ; Entity ID: "
//			  << hp->GetOwner()->GetID()
//			  << std::endl;
//}