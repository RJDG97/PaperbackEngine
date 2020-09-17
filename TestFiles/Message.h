#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum class MessageIDTypes
{
	None = 0,
	Exit,
	Rotate,
	HP,

	// Sound System
	BGM_Play,
	BGM_Stop,
	BGM_Pause,
	BGM_Mute,
	BGM_Completed,
	BGM_Reload,

	// Game State System
	GSM_ChangeState,
	GSM_PushState,
	GSM_PopState,
	GSM_PauseState // TBC
};

class Message
{
public:
	MessageIDTypes MessageID;

	Message(MessageIDTypes id) : MessageID{ id }
	{}

	virtual ~Message() = default;
};

class Entity_Message : public Message
{
public:
	size_t entityone, entitytwo;

	Entity_Message(MessageIDTypes id, size_t id_1 = 0, size_t id_2 = 0) : Message{ id }, 
																		  entityone{ id_1 }, 
																		  entitytwo{ id_2 }
	{}

	virtual ~Entity_Message() = default;
};

#endif