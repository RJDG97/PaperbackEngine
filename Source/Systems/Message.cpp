#include "Systems/Message.h"

Message::Message(MessageIDTypes id) : message_id_{ id }
{}

MessagePhysics_Motion::MessagePhysics_Motion(MessageIDTypes id, Vector2D new_vector) :
	Message{ id },
	new_vec_{ new_vector }
{}

Message_Input::Message_Input(MessageIDTypes id, size_t input) : 
	Message{ id },
	input_{ input }
{}

Message_PlayerInput::Message_PlayerInput(MessageIDTypes id, size_t input_flag) :
	Message{ id },
	input_flag_{ input_flag }
{}

MessageBGM_Play::MessageBGM_Play(const std::string file_id) :
	Message(MessageIDTypes::BGM_PLAY),
	file_id_ { file_id }
{}

MessageBGM_Stop::MessageBGM_Stop(const std::string file_id) :
	Message(MessageIDTypes::BGM_STOP),
	file_id_{ file_id }
{}

Message_Button::Message_Button(size_t button_index) :
	Message{ MessageIDTypes::BUTTON },
	button_index_{ button_index }
{}