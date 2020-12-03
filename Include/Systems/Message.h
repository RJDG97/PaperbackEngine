/**********************************************************************************
*\file         Message.h
*\brief        Contains declaration of the different messages in the game
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include "MathLib/Vector2D.h"

const unsigned char UP_FLAG = 1;		// 0000 0000 0000 0001
const unsigned char DOWN_FLAG = 2;		// 0000 0000 0000 0010
const unsigned char LEFT_FLAG = 4;		// 0000 0000 0000 0100
const unsigned char RIGHT_FLAG = 8;		// 0000 0000 0000 1000

const unsigned char W_FLAG = 16;		// 0000 0000 0001 0000
const unsigned char S_FLAG = 32;		// 0000 0000 0010 0000
const unsigned char A_FLAG = 64;		// 0000 0000 0100 0000
const unsigned char D_FLAG = 128;		// 0000 0000 1000 0000

class GameState;

enum class MessageIDTypes
{
	// Testing
	NONE = 0,
	EXIT,
	ROTATE,
	HP,
	DEBUG_ALL,

	// Sound System
	BGM_PLAY,
	BGM_STOP,
	BGM_PAUSE,
	BGM_RESUME,
	BGM_MUTE,

	// Game State System
	GSM_CHANGESTATE,
	GSM_PUSHSTATE,
	GSM_POPSTATE,
	GSM_WIN, // TBC
	GSM_LOSE,

	// Message to Game System
	M_MOVEMENT,
	M_BUTTON_PRESS,
	M_BUTTON_TRIGGERED,
	M_MOUSE_PRESS,

	// Physics System
	PHY_UPDATE_ACCEL,
	PHY_UPDATE_VEL,

	// Camera movement
	CAM_UPDATE_POS,
	C_MOVEMENT,

	// Change animation
	CHANGE_ANIMATION_1,
	CHANGE_ANIMATION_2,
	FLIP_SPRITE_X,
	FLIP_SPRITE_Y,

	// Factory
	FTY_PURGE,
	FTY_DELETE,

	// Button input
	BUTTON
};

// Message Interface
struct Message
{
public:
	MessageIDTypes message_id_;

/******************************************************************************/
/*!
  \fn Message();

  \brief The constructor for the Message struct. It takes in an enum that 
		 identifies to receiving systems the id of the message.
*/
/******************************************************************************/
	Message(MessageIDTypes id);
	virtual ~Message() = default;
};

struct MessagePhysics_Motion : public Message {

	Vec2 new_vec_;

/******************************************************************************/
/*!
  \fn MessagePhysics_Motion()

  \brief Initializes a message with a custom message ID and new vector
*/
/******************************************************************************/
	MessagePhysics_Motion(MessageIDTypes id, Vector2D new_vector);
};

struct Message_Button : public Message 
{
	size_t button_index_;
	
/******************************************************************************/
/*!
  \fn Message_Button()

  \brief Initializes a message with a button index for receiving systems to handle
*/
/******************************************************************************/
	Message_Button(size_t button_index);
};

struct Message_Input : public Message
{
	size_t input_;

/******************************************************************************/
/*!
  \fn Message_Input()

  \brief Initializes a message with a custon message ID and movement key
*/
/******************************************************************************/
	Message_Input(MessageIDTypes id, size_t input);
};

struct Message_PlayerInput : public Message
{
	size_t input_flag_;

/******************************************************************************/
/*!
  \fn Message_PlayerInput()

  \brief Initializes a message with a custom message ID and movement flag
*/
/******************************************************************************/
	Message_PlayerInput(MessageIDTypes id, size_t input_flag);
};

struct MessageBGM_Play : public Message 
{
	std::string file_id_;

/******************************************************************************/
/*!
  \fn MessageBGM_Play()

  \brief Initializes a message with the name of sound file to be played
*/
/******************************************************************************/
	MessageBGM_Play(const std::string file_id);
};

struct MessageBGM_Stop : public Message
{
	std::string file_id_;

	/******************************************************************************/
	/*!
	  \fn MessageBGM_Stop()

	  \brief Initializes a message with the name of sound file to be stopped
	*/
	/******************************************************************************/
	MessageBGM_Stop(const std::string file_id);
};

///Message to tell the game to quit
class MessageQuit : public Message
{
public:
/******************************************************************************/
/*!
  \fn MessageQuit()

  \brief Initializes a message with the exit enum
*/
/******************************************************************************/
	MessageQuit() : Message(MessageIDTypes::EXIT) {};
};
#endif