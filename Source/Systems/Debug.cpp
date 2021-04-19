/**********************************************************************************
*\file         Debug.cpp
*\brief        Contains definition of functions and variables used for
*			   the Debug System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/Debug.h"
#include <iostream>
#include "Engine/Core.h"

EngineDebug* EngineDebug::d_instance_ = 0;

EngineDebug::EngineDebug() {
	//my_file_.open("Resources/Debug.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
}

EngineDebug::~EngineDebug() {
	//std::cout << "Debug dtor called" << std::endl;
	/*if (my_file_.is_open()) {
		my_file_.close();
	}*/
}

void EngineDebug::WriteDebugMessage(const std::string& str) {

	/*#if defined(DEBUG) | defined(_DEBUG)
	if (my_file_.is_open()) {
		my_file_ << str;
	}
	#endif
	*/
	(void)str;
}

void EngineDebug::SaveDebug() {

	/*my_file_.close();

	my_file_.open("Resources/Debug.txt", std::ios_base::app);*/
}

bool EngineDebug::MyAssert(const char* expr_str, const char* file, size_t line, const char* return_message) {

	(void)expr_str;
	(void)file;
	(void)line;
	(void)return_message;

	Message m{ MessageIDTypes::EXIT };
	CORE->BroadcastMessage(&m);

	return true;
}