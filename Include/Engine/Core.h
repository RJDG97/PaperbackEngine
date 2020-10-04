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
	CoreEngine();
	~CoreEngine() = default;
	///Update all the systems until the game is no longer active.
	void GameLoop();
	///Destroy all systems in reverse order that they were added.
	void DestroySystems();
	//Destroy all managers in reverse order that they were added
	void DestroyManagers();
	///Broadcasts a message to all systems.
	void BroadcastMessage(Message* m);
	///Initializes all systems in the game.
	void Initialize();

	template <typename SystemType>
	void AddSystem() {
		for (SystemIt begin = systems_.begin(); begin != systems_.end(); ++begin) {
			assert(begin->first != typeid(SystemType).name() && "System already exists");
		}
		systems_.push_back({ typeid(SystemType).name(), new SystemType() });

		// Log system message to "Source/Debug.txt"
		M_DEBUG->WriteDebugMessage("Adding System: " + systems_.back().first + "\n");
	}

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
		assert((begin != systems_.end()) && "System does not exist");
	}

	template <typename ManagerType>
	void AddManager() {
		assert(managers_.find(typeid(ManagerType).name()) == managers_.end() && "Manager already exists");

		// Log system message to "Source/Debug.txt"
		std::stringstream str;
		str << "Adding Manager: " << typeid(ManagerType).name() << "\n";
		M_DEBUG->WriteDebugMessage(str.str());

		managers_[typeid(ManagerType).name()] = new ManagerType;
	}

	template <typename ManagerType>
	ManagerType* GetManager() {
		assert(managers_.find(typeid(ManagerType).name()) != managers_.end() && "Manager does not exist");
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

extern CoreEngine* CORE;

#endif