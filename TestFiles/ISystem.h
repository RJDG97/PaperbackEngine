#ifndef _ISYSTEM_H_
#define _ISYSTEM_H_

#include <string>

class ISystem {
public:
	virtual void init() {}
	virtual void update() = 0;
	virtual std::string GetName() = 0;

	virtual ~ISystem() = default;
};

#endif