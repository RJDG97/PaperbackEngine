/**********************************************************************************
*\file         Debug.h
*\brief        Contains declaration of functions and variables used for
*			   the Debug System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
  \fn MyAssert()

  \brief Custom assertion function, stores the expression, file, line and
		 error message of when an assert fails
*/
/******************************************************************************/
	bool MyAssert(const char* expr_str, const char* file, size_t line, const char* return_message);

/******************************************************************************/
/*!
  \fn ~EngineDebug()

  \brief Used to call the closure of the file that was opened in constructor
*/
/******************************************************************************/
	~EngineDebug();
};

#define M_DEBUG EngineDebug::GetInstance()

#define DEBUG_ASSERT( exp, str ) \
		do { if( (!(exp)) && M_DEBUG->MyAssert( #exp, __FILE__, __LINE__, str)) __debugbreak(); } while (0)
#endif