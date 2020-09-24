#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include "Vector2D.h"

class GameState;

enum class MessageIDTypes
{
	// Testing
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
	GSM_PauseState, // TBC

	// Message to Game System
	M_ButtonPress,

	// Physics System
	PHY_UpdateAccel,

	// Factory
	FTY_Purge,
	FTY_Delete
};

// Message Interface
struct Message
{
public:
	MessageIDTypes message_id_;

	Message(MessageIDTypes id);
	virtual ~Message() = default;
};

// Entity Message Interface
struct Entity_Message : public Message
{
	size_t entity_one_, entity_two_;

	Entity_Message(MessageIDTypes id, size_t id_1 = 0, size_t id_2 = 0);
	virtual ~Entity_Message() = default;
};

struct MessagePhysics_Accel : public Message
{
	Vector2D new_acceleration_;

	MessagePhysics_Accel(MessageIDTypes id, Vector2D new_acceleration);
};

struct Message_Input : public Message
{
	int input_;

	Message_Input(MessageIDTypes id, int input);
};

struct MessageBGM_Play : public Message 
{
	std::string file_id_;

	MessageBGM_Play(const std::string file_id);
};

// can remove possibly
struct MessageRotation : public Entity_Message
{
	//GOC * CollidedWith;

	MessageRotation(size_t entity_id);
};

// can remove possibly
struct MessageHPDecre : public Entity_Message
{
	//GOC * CollidedWith;

	MessageHPDecre(size_t entity_id);
};

struct Message_CustomState : public Message
{
	GameState* state_;
	Message_CustomState(GameState* state, MessageIDTypes message_type);
};
#endif