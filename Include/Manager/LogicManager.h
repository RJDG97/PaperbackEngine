/**********************************************************************************
*\file         LogicManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Logic Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _LOGIC_MANAGER_H_
#define _LOGIC_MANAGER_H_
#include <map>
#include "Entity/Entity.h"
#include "Manager/IManager.h"
#include "Systems/Debug.h"
#include <iostream>
#include <type_traits>


template <typename T>
using remove_cv = std::conditional_t<
	// If T == &
	std::is_reference_v<T>,
	// Is &
	std::conditional_t<
		// If *& (Because param was * to &&)
		std::is_pointer_v<std::remove_reference_t<T>>,
		// Remove &, return T*
		std::remove_reference_t<T>,
		// Return T*
		T>,
	// Is not &, return T
	T>;


class LogicManager : public IManager
{
private:

	template <typename... Args>
	static inline std::map<std::string, void(*)(Args...)> logic_map_;

	/******************************************************************************/
	/*!
		\fn RegisterLogic()

		\brief Registers a function pointer to a string name
	*/
	/******************************************************************************/
	template <typename... Args>
	void RegisterLogic(const std::string& name, void(*function)(Args...)) {

		// Throw assertion if if the function has already been added before
		auto it = logic_map_<remove_cv<Args>...>.find(name);
		DEBUG_ASSERT(it == logic_map_<remove_cv<Args>...>.end(), "Function has already been added");

		DEBUG_ASSERT(function, "Function is nullptr!");
		logic_map_<remove_cv<Args>...>[name] = function;
	}

public:
	/******************************************************************************/
	/*!
	  \fn LogicManager()

	  \brief Default constructor for LogicManager
	*/
	/******************************************************************************/
	LogicManager();

	/******************************************************************************/
	/*!
	  \fn Init()

	  \brief Initializes any required data members within the LogicManager
	*/
	/******************************************************************************/
	void Init() override;

	/******************************************************************************/
	/*!
		\fn Exec()

		\brief Executes a function with the matching param signatures & matching string
	*/
	/******************************************************************************/
	template <typename... Args>
	void Exec(const std::string& name, Args&&... args) {

		DEBUG_ASSERT(logic_map_<remove_cv<Args>...>[name], "Function is nullptr!");
		logic_map_<remove_cv<Args>...>[name](std::forward<Args>(args)...);
	}
};

#endif