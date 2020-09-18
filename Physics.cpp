#include  "Physics.h"
#include <iostream>

Physics* PHYSICS;

Physics::Physics() {
	PHYSICS = this;

}

void Physics::init() {
	// Temporary addition for debugging
	FACTORY->AddComponentCreator("Transform", new ComponentCreatorType<Transform>( ComponentTypes::TRANSFORM));
	FACTORY->AddComponentCreator("Health", new ComponentCreatorType<Health>(ComponentTypes::HEALTH));
}

void Physics::update(float frametime) {
	
}

void Physics::DetectCollision() {
	
}

void Physics::PublishResults() {
	for (PosIt it = Transforms.begin(); it != Transforms.end(); ++it) {
		(it)->PublishResults();
	}
}

void Physics::DebugDraw() {
	
}

void Physics::SendMessageD(Message* msg) {
	
	std::cout << "Message received by Physics" << std::endl;

	if (msg->MessageID == MessageIDTypes::Rotate) {
		std::cout << "prepare to rotate" << std::endl;

		MessageRotation* m = dynamic_cast<MessageRotation*>(msg);

		for (PosIt it = PHYSICS->Transforms.begin(); it != PHYSICS->Transforms.end(); ++it) {

			// look into swapping from std::list to std::unordered_map<entityid, component*> to allow for std::find usage
			if (it->GetOwner()->GetID() == m->entityone) {
				Rotate(&*it);
				break;
			}
		}
	}

	if (msg->MessageID == MessageIDTypes::HP) {

		std::cout << "prepare to decrement hp" << std::endl;

		MessageHPDecre* m = dynamic_cast<MessageHPDecre*>(msg);

		for (HPIt it = PHYSICS->HPs.begin(); it != PHYSICS->HPs.end(); ++it) {

			if (it->GetOwner()->GetID() == m->entityone) {

				DecreaseHP(&*it);
				break;
			}
		}
	}
}

void Physics::Rotate(Transform* pos) {

	pos->rotation_ += 0.2f;

	std::cout << "Message recieved, rotation success, currently at: " 
			  << pos->rotation_
			  << " ; Entity ID: " 
			  << pos->GetOwner()->GetID()
			  << std::endl;
}

void Physics::DecreaseHP(Health* hp) {

	hp->current_health_ -= 1;

	std::cout << "Message recieved, health decremented successfully, currently at: "
			  << hp->current_health_
			  << " ; Entity ID: "
			  << hp->GetOwner()->GetID()
			  << std::endl;
}