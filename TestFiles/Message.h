#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum MessageIDTypes
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
	BGM_Reload
};

class Message
{
public:
	MessageIDTypes MessageID;

	Message(MessageIDTypes id) : MessageID{ id }
	{}
	virtual ~Message() = default;
};

#endif