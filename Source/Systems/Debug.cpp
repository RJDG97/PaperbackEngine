#include "Systems/Debug.h"
#include <iostream>

EngineDebug* EngineDebug::d_instance_ = 0;

EngineDebug::EngineDebug() {
	my_file_.open("Resources/Debug.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
}

EngineDebug::~EngineDebug() {
	std::cout << "Debug dtor called" << std::endl;
	my_file_.close();
}

void EngineDebug::WriteDebugMessage(const std::string& str) {
	
	if (my_file_.is_open()) {
		my_file_ << str;
	}

}