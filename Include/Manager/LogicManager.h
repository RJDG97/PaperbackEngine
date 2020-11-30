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

	template <typename... Args/*, typename... Params*/>
	void Exec(const std::string& name, Args&&... args) {

		DEBUG_ASSERT(logic_map_<remove_cv<Args>...>[name], "Function is nullptr!");
		logic_map_<remove_cv<Args>...>[name](std::forward<Args>(args)...);
	}
};

#endif