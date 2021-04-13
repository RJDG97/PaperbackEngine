/**********************************************************************************
*\file         Core.h
*\brief        Contains declaration of functions and variables used for
*			   the Core Engine System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _CORE_ENGINE_H_
#define _CORE_ENGINE_H_

#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Systems/Debug.h"
#include "Systems/FrameRateController.h"

#include "Manager/IManager.h"

#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <assert.h>
#include <sstream>

class CoreEngine
{
public:
/******************************************************************************/
/*!
  \fn CoreEngine()

  \brief Constructor for CoreEngine
*/
/******************************************************************************/
	CoreEngine();

/******************************************************************************/
/*!
  \fn ~CoreEngine()

  \brief Default destructor for CoreEngine
*/
/******************************************************************************/
	~CoreEngine() = default;

/******************************************************************************/
/*!
  \fn GameLoop()

  \brief The main game loop, it cycles through Update()'s of all systems
*/
/******************************************************************************/
	void GameLoop();

/******************************************************************************/
/*!
  \fn GetGlobalScale()

  \brief Returns the default current scale for translation and scaling
*/
/******************************************************************************/
	float GetGlobalScale() const;

/******************************************************************************/
/*!
  \fn BroadcastMessage()

  \brief Used to broadcast messages to all systems derived from the ISystem
		 interface class
*/
/******************************************************************************/
	void BroadcastMessage(Message* m);
	
/******************************************************************************/
/*!
  \fn Initialize()

  \brief Used to initialize all systems and managers that were attached to the
		 Core Engine System
*/
/******************************************************************************/
	void Initialize();

/******************************************************************************/
/*!
  \fn AddSystem<T>()

  \brief Used to add a system to the Core Engine by passing in the template 
		 parameter
*/
/******************************************************************************/
	template <typename SystemType>
	void AddSystem() {
		for (SystemIt begin = systems_.begin(); begin != systems_.end(); ++begin) {

			DEBUG_ASSERT((begin->first != typeid(SystemType).name()), "System already exists");
		}
		systems_.push_back({ typeid(SystemType).name(), std::make_shared<SystemType>() });

		// Log system message to "Source/Debug.txt"
		M_DEBUG->WriteDebugMessage("Adding System: " + systems_.back().first + "\n");
	}

/******************************************************************************/
/*!
  \fn GetSystem<T>()

  \brief Used to retrieve a system that was attached to the Core Engine by
		 passing in the template parameter
*/
/******************************************************************************/
	template <typename SystemType>
	std::shared_ptr<SystemType> GetSystem() {

		SystemIt begin = systems_.begin();
		for (; begin != systems_.end(); ++begin) {
			if (begin->first == typeid(SystemType).name()) {
				if (debug_) {
					// Log system message to "Source/Debug.txt"
					M_DEBUG->WriteDebugMessage("Getting System: " + begin->first + "\n");
				}
				return std::reinterpret_pointer_cast<SystemType>(begin->second);
			}
		}

		DEBUG_ASSERT((begin != systems_.end()), "System does not exist");
		return nullptr;
	}

/******************************************************************************/
/*!
  \fn AddManager<T>()

  \brief Used to add a manager to the Core Engine by passing in the template 
		 parameter
*/
/******************************************************************************/
	template <typename ManagerType>
	void AddManager() {

		DEBUG_ASSERT((managers_.find(typeid(ManagerType).name()) == managers_.end()), "Manager already exists");
		// Log system message to "Source/Debug.txt"
		std::stringstream str;
		str << "Adding Manager: " << typeid(ManagerType).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());

		managers_[typeid(ManagerType).name()] = std::make_shared<ManagerType>();
	}

/******************************************************************************/
/*!
  \fn GetManager<T>()

  \brief Used to retrieve a manager that was attached to the Core Engine by
		 passing in the template parameter
*/
/******************************************************************************/
	template <typename ManagerType>
	std::shared_ptr<ManagerType> GetManager() {

		DEBUG_ASSERT((managers_.find(typeid(ManagerType).name()) != managers_.end()), "Manager does not exist");

		ManagerIt return_val = managers_.find(typeid(ManagerType).name());

		if (debug_) {
			// Log system message to "Source/Debug.txt"
			std::stringstream str;
			str << "Getting Manager: " << typeid(ManagerType).name() << "\n";
			M_DEBUG->WriteDebugMessage(str.str());
		}
		
		return std::reinterpret_pointer_cast<ManagerType>(return_val->second);
	}


/******************************************************************************/
/*!
  \fn GetCorePauseStatus()

  \brief Get pause status
*/
/******************************************************************************/
	bool GetCorePauseStatus();

/******************************************************************************/
/*!
  \fn ResetCorePauseStatus()

  \brief Reset pause status
*/
/******************************************************************************/
	void ResetCorePauseStatus();

/******************************************************************************/
/*!
  \fn ToggleCorePauseStatus()

  \brief Toggle pause status
*/
/******************************************************************************/
	void ToggleCorePauseStatus();



/******************************************************************************/
/*!
  \fn GetGamePauseStatus()

  \brief Get game pause status
*/
/******************************************************************************/
	bool GetGamePauseStatus();

/******************************************************************************/
/*!
  \fn 	void ResetGamePauseStatus();
()

  \brief Reset game pause status
*/
/******************************************************************************/
	void ResetGamePauseStatus();

/******************************************************************************/
/*!
  \fn ToggleGamePauseStatus()

  \brief Toggle game pause status
*/
/******************************************************************************/
	void ToggleGamePauseStatus();

/******************************************************************************/
/*!
  \fn SetGameActiveStatus()

  \brief Set game pause status
*/
/******************************************************************************/
	void SetGameActiveStatus(bool status);



/******************************************************************************/
/*!
  \fn GetGodMode()

  \brief Get god mode
*/
/******************************************************************************/
	bool GetGodMode();

/******************************************************************************/
/*!
  \fn SetGodMode()

  \brief Set god mode
*/
/******************************************************************************/
	void SetGodMode(bool status);

/******************************************************************************/
/*!
  \fn ToggleGodMode()

  \brief Toggle god mode
*/
/******************************************************************************/
	void ToggleGodMode();

/******************************************************************************/
/*!
  \fn ResetGodMode()

  \brief Reset god mode
*/
/******************************************************************************/
	void ResetGodMode();

/******************************************************************************/
/*!
  \fn GetMovementLock()

  \brief Get status of whether movement is locked
*/
/******************************************************************************/
	bool GetMovementLock();

/******************************************************************************/
/*!
  \fn SetGetMovementLock()

  \brief Set movement lock
*/
/******************************************************************************/
	void SetMovementLock(bool status = false);

private:

	bool debug_;
	bool pause_;
	bool game_pause_;
	bool god_mode_;
	bool movement_lock_;

	// Tracks all the systems the game uses
	using SystemIt = std::vector< std::pair<std::string, std::shared_ptr<ISystem>> >::iterator;
	std::vector< std::pair<std::string, std::shared_ptr<ISystem>> > systems_;

	// Tracks all the managers the system uses
	using ManagerIt = std::unordered_map<std::string, std::shared_ptr<IManager>>::iterator;
	std::unordered_map<std::string, std::shared_ptr<IManager>> managers_;

	//Is the game running (true) or being shut down (false)?
	bool b_game_active_;

	const float global_scale_;
};

extern std::unique_ptr<CoreEngine> CORE;

#endif