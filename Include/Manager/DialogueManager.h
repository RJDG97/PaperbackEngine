#pragma once
#include "Manager/IManager.h"
#include <vector>
#include <string>

class DialogueContent
{
	std::string portrait_;
	std::string name_;
	std::string speech_;
};

class Dialogue
{
	std::vector<DialogueContent> contents_;
};

class DialogueManager : public IManager {

public:

	virtual void Init();

	virtual void Update(float frametime);

};
