#ifndef _ISYSTEM_H_
#define _ISYSTEM_H_

#include <string>
#include "Message.h"

class ISystem {
public:
	virtual void init() {}
	virtual void update(float frametime) = 0;
	virtual std::string GetName() = 0;
	virtual void SendMessageD(Message* m) = 0;

	virtual ~ISystem() = default;
};

#endif