#include "Systems/Debug.h"
#include <iostream>
#include "Engine/Core.h"

EngineDebug* EngineDebug::d_instance_ = 0;

EngineDebug::EngineDebug() {
	my_file_.open("Resources/Debug.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
}

EngineDebug::~EngineDebug() {
	std::cout << "Debug dtor called" << std::endl;
	if (my_file_.is_open()) {
		my_file_.close();
	}
}

void EngineDebug::WriteDebugMessage(const std::string& str) {
	
	if (my_file_.is_open()) {
		my_file_ << str;
	}
}

void EngineDebug::SaveDebug() {

	my_file_.close();

	my_file_.open("Resources/Debug.txt", std::ios_base::app);
}

bool EngineDebug::MyAssert(const char* expr_str, const char* file, size_t line, const char* return_message) {
	//prepare string format to print into file
	std::stringstream stream;

	stream << "\nERROR HAS OCCURRED BEGIN LOG\nError Expression: " << expr_str 
			<< "\nFile: " << file << "\nLine: " << line << "\nError Message: " << return_message
			<< "\nERROR HAS OCCURRED END LOG\n";

	std::cerr << stream.str();

	WriteDebugMessage(stream.str());

	SaveDebug();

	Message m{ MessageIDTypes::EXIT };
	CORE->BroadcastMessage(&m);

	return true;
}