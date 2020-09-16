#include  "Physics.h"
#include <iostream>

Physics* PHYSICS;

Physics::Physics() {
	PHYSICS = this;

}

void Physics::init() {
	FACTORY->AddComponentCreator("Transform", new ComponentCreatorType<Transform>( static_cast<ComponentTypes>(TRANSFORM) ));
	FACTORY->AddComponentCreator("Health", new ComponentCreatorType<Health>(static_cast<ComponentTypes>(HEALTH)));
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
		for (PosIt it = PHYSICS->Transforms.begin(); it != PHYSICS->Transforms.end(); ++it) {

			Rotate(&*it);
		}
	}

	if (msg->MessageID == MessageIDTypes::HP) {

		std::cout << "prepare to decrement hp" << std::endl;
		for (HPIt it = PHYSICS->HPs.begin(); it != PHYSICS->HPs.end(); ++it) {

			DecreaseHP(&*it);
		}
	}
}

void Physics::Rotate(Transform* pos) {

	pos->_rotation += 0.2;

	std::cout << "Message recieved, rotation success, currently at: " << pos->_rotation << std::endl;
}

void Physics::DecreaseHP(Health* hp) {

	hp->currentHealth -= 1;

	std::cout << "Message recieved, health decremented successfully, currently at: " << hp->currentHealth << std::endl;
}