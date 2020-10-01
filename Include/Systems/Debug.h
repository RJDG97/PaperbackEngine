#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>
#include <fstream>
#include <sstream>

class EngineDebug
{
	static EngineDebug* d_instance_;
	std::fstream my_file_;

	EngineDebug();
public:
	static EngineDebug* GetInstance() {
		if (!d_instance_) { d_instance_ = new EngineDebug; }
		return d_instance_;
	}
	static void DeleteInstance() {
		if (d_instance_)
			delete d_instance_;
	}

	void WriteDebugMessage(const std::string& str);

	~EngineDebug();
};

#define M_DEBUG EngineDebug::GetInstance()

#endif



/*
#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
	if (!expr)
	{
		std::cerr << "Assert failed:\t" << msg << "\n"
			<< "Expected:\t" << expr_str << "\n"
			<< "Source:\t\t" << file << ", line " << line << "\n";
		abort();
	}
}
*/