#include "Systems/Message.h"

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

Message_PlayerInput::Message_PlayerInput(MessageIDTypes id, unsigned char input_flag) :
	Message{ id },
	input_flag_{ input_flag }
{}

MessageBGM_Play::MessageBGM_Play(const std::string file_id) :
	Message(MessageIDTypes::BGM_PLAY),
	file_id_ { file_id }
{}

MessageRotation::MessageRotation(size_t entity_id) : 
	Entity_Message{ MessageIDTypes::ROTATE, entity_id } 
{}

MessageHPDecre::MessageHPDecre(size_t entity_id) : 
	Entity_Message{ MessageIDTypes::HP, entity_id } 
{}

Message_CustomState::Message_CustomState(GameState* state, MessageIDTypes message_type) : 
	Message{ message_type },
	state_{ state }
{}