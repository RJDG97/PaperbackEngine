#pragma once
#ifndef _HEALTH_H_
#define _HEALTH_H_

#include "IComponent.h"
#include <list>

class Health : public Component 
{
public:
	int currentHealth;
	int maximumHealth;

	Health();
	void init();
	void PublishResults();
};

#endif