#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

class GameState;

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

// Message Interface
struct Message
{
public:
	MessageIDTypes MessageID;

	Message(MessageIDTypes id);
	virtual ~Message() = default;
};

// Entity Message Interface
struct Entity_Message : public Message
{
	size_t entityone, entitytwo;

	Entity_Message(MessageIDTypes id, size_t id_1 = 0, size_t id_2 = 0);
	virtual ~Entity_Message() = default;
};


struct MessageBGM_Play : public Message 
{
	std::string _fileID;

	MessageBGM_Play(const std::string fileID);
};

// can remove possibly
struct MessageRotation : public Entity_Message
{
	//GOC * CollidedWith;

	MessageRotation(size_t entityID);
};

// can remove possibly
struct MessageHPDecre : public Entity_Message
{
	//GOC * CollidedWith;

	MessageHPDecre(size_t entityID);
};

struct Message_CustomState : public Message
{
	GameState* _state;
	Message_CustomState(GameState* state, MessageIDTypes messagetype);
};
#endif