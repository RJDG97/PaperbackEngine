#include "Message.h"

Message::Message(MessageIDTypes id) : message_id_{ id }
{}

Entity_Message::Entity_Message(MessageIDTypes id, size_t id_1, size_t id_2) : 
	Message{ id },
	entity_one_{ id_1 },
	entity_two_{ id_2 }
{}

MessagePhysics_Motion::MessagePhysics_Motion(MessageIDTypes id, Vector2D new_vector) :
	Message{ id },
	new_vec_{ new_vector }
{}

Message_Input::Message_Input(MessageIDTypes id, int input) : 
	Message{ id },
	input_{ input }
{}

MessageBGM_Play::MessageBGM_Play(const std::string file_id) :
	Message(MessageIDTypes::BGM_Play),
	file_id_ { file_id }
{}

MessageRotation::MessageRotation(size_t entity_id) : 
	Entity_Message{ MessageIDTypes::Rotate, entity_id } 
{}

MessageHPDecre::MessageHPDecre(size_t entity_id) : 
	Entity_Message{ MessageIDTypes::HP, entity_id } 
{}

Message_CustomState::Message_CustomState(GameState* state, MessageIDTypes message_type) : 
	Message{ message_type },
	state_{ state }
{}