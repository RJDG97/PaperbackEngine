#pragma once
#ifndef _HEALTH_H_
#define _HEALTH_H_

#include "IComponent.h"
#include <list>

// Contains data about the health of an entity that the component will be attached to
class Health : public Component 
{
	int current_health_;
	int maximum_health_;
public:
	friend class Physics; // Temporary friend class for testing purposes

	Health();

	void Init();
	void PublishResults();
	//Serialize
};

#endif