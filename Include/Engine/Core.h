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
  \fn DestroyManagers()

  \brief Used to deallocate all systems that were attached to the Core Engine
		 System
*/
/******************************************************************************/
	void DestroySystems();

/******************************************************************************/
/*!
  \fn DestroyManagers()

  \brief Used to deallocate all managers that were attached to the Core Engine
		 System
*/
/******************************************************************************/
	void DestroyManagers();

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
		systems_.push_back({ typeid(SystemType).name(), new SystemType() });

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
	SystemType* GetSystem() {

		SystemIt begin = systems_.begin();
		for (; begin != systems_.end(); ++begin) {
			if (begin->first == typeid(SystemType).name()) {
				if (debug_) {
					// Log system message to "Source/Debug.txt"
					M_DEBUG->WriteDebugMessage("Getting System: " + begin->first + "\n");
				}
				return dynamic_cast<SystemType*>(begin->second);
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

		managers_[typeid(ManagerType).name()] = new ManagerType;
	}

/******************************************************************************/
/*!
  \fn GetManager<T>()

  \brief Used to retrieve a manager that was attached to the Core Engine by
		 passing in the template parameter
*/
/******************************************************************************/
	template <typename ManagerType>
	ManagerType* GetManager() {

		DEBUG_ASSERT((managers_.find(typeid(ManagerType).name()) != managers_.end()), "Manager does not exist");

		IManager* return_val = managers_.find(typeid(ManagerType).name())->second;

		if (debug_) {
			// Log system message to "Source/Debug.txt"
			std::stringstream str;
			str << "Getting Manager: " << typeid(ManagerType).name() << "\n";
			M_DEBUG->WriteDebugMessage(str.str());
		}
		
		return dynamic_cast<ManagerType*>(return_val);
	}

private:

	bool debug_;

	// Tracks all the systems the game uses
	using SystemIt = std::vector< std::pair<std::string, ISystem*> >::iterator;
	std::vector< std::pair<std::string, ISystem*> > systems_;

	// Tracks all the managers the system uses
	using ManagerIt = std::unordered_map<std::string, IManager*>::iterator;
	std::unordered_map<std::string, IManager*> managers_;

	////The last time the game was updated
	//unsigned LastTime;
	//Is the game running (true) or being shut down (false)?
	bool b_game_active_;
};

extern std::unique_ptr<CoreEngine> CORE;

#endif