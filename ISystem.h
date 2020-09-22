#ifndef _ISYSTEM_H_
#define _ISYSTEM_H_

#include <string>
#include "Message.h"

class ISystem {
public:

	//init function called to initialise a system
	virtual void Init() {}

	//contains logic executed during the update loop of a game
	virtual void Update(float frametime) = 0;

	//returns the name of the system for debug use
	virtual std::string GetName() = 0;

	//function more akin to "What to do when message is received" for internal logic
	virtual void SendMessageD(Message* m) = 0;

	virtual ~ISystem() = default;
};

#endif