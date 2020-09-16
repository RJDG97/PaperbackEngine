#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum MessageIDTypes
{
	None = 0,
	Exit,
	Rotate,
	HP
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