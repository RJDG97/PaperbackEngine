#include "Message.h"

Message::Message(MessageIDTypes id) : MessageID{ id }
{}

Entity_Message::Entity_Message(MessageIDTypes id, size_t id_1, size_t id_2) : Message{ id },
	entityone{ id_1 },
	entitytwo{ id_2 }
{}

MessageBGM_Play::MessageBGM_Play(const std::string fileID) :
	Message(MessageIDTypes::BGM_Play),
	_fileID{ fileID }
{}

MessageRotation::MessageRotation(size_t entityID) : Entity_Message{ MessageIDTypes::Rotate, entityID } 
{}

MessageHPDecre::MessageHPDecre(size_t entityID) : Entity_Message{ MessageIDTypes::HP, entityID } 
{}

Message_CustomState::Message_CustomState(GameState* state, MessageIDTypes messagetype) : Message{ messagetype },
	_state{ state }
{}