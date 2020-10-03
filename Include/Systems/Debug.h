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

/******************************************************************************/
/*!
  \fn EngineDebug()

  \brief Opens the file Resources/Debug.txt
*/
/******************************************************************************/
	EngineDebug();
public:
/******************************************************************************/
/*!
  \fn GetInstance()

  \brief Creates a new instance of EngineDebug if it does not yet exist
		 Returns a pointer to the instance if it already exists
*/
/******************************************************************************/
	static EngineDebug* GetInstance() {
		if (!d_instance_) { d_instance_ = new EngineDebug; }
		return d_instance_;
	}

/******************************************************************************/
/*!
  \fn DeleteInstance()

  \brief Used to free the memory allocated to the EngineDebug instance;
*/
/******************************************************************************/
	static void DeleteInstance() {
		if (d_instance_)
			delete d_instance_;
	}

/******************************************************************************/
/*!
  \fn WriteDebugMessage()

  \brief Message passed into the function will be logged into Resources/Debug.txt
*/
/******************************************************************************/
	void WriteDebugMessage(const std::string& str);

/******************************************************************************/
/*!
  \fn SaveDebug()

  \brief The file is closed to save all logs made and reopened to allow for more
		 to be logged
*/
/******************************************************************************/
	void SaveDebug();

/******************************************************************************/
/*!
  \fn ~EngineDebug()

  \brief Used to call the closure of the file that was opened in constructor
*/
/******************************************************************************/
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