#ifndef _CORE_ENGINE_H_
#define _CORE_ENGINE_H_

#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Systems/FrameRateController.h"

#include "Manager/IManager.h"

#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <assert.h>

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
	///Adds a new system to the game.
	//void AddSystem(ISystem* system);
	///Adds a new manager to the system.
	//void AddManager(IManager* manager);
	///Initializes all systems in the game.
	void Initialize();

	template <typename SystemType>
	void AddSystem() {
		for (SystemIt begin = systems_.begin(); begin != systems_.end(); ++begin) {
			assert(begin->first != typeid(SystemType).name() && "System already exists");
		}
		systems_.push_back({ typeid(SystemType).name(), new SystemType() });
		

		/*
		//systems_.push_back(new SystemType);
		assert(systems_.find(typeid(SystemType).name()) == systems.end() && "System already exists");
		systems_[typeid(SystemType).name()] = new SystemType;
		*/
	}

	template <typename SystemType>
	SystemType* GetSystem() {
		/*
		assert(systems_.find(typeid(SystemType).name()) != systems_.end() && "System does not exist");
		ISystem* return_val = systems_.find(typeid(SystemType).name())->second;
		return dynamic_cast<SystemType*>(return_val);
		*/

		SystemIt begin = systems_.begin();
		for (; begin != systems_.end(); ++begin) {
			if (begin->first == typeid(SystemType).name()) {
				return dynamic_cast<SystemType*>(begin->second);
			}
		}
		assert((begin != systems_.end()) && "System does not exist");

		/*
		for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
			if (system->second->GetName() == system_name) {
				return dynamic_cast<SystemType*>(*system->second);
			}
		}
		return nullptr;
		*/
	}

	template <typename ManagerType>
	void AddManager() {
		assert(managers_.find(typeid(ManagerType).name()) == managers_.end() && "Manager already exists");
		managers_[typeid(ManagerType).name()] = new ManagerType;
	}

	template <typename ManagerType>
	ManagerType* GetManager() {
		assert(managers_.find(typeid(ManagerType).name()) != managers_.end() && "Manager does not exist");
		IManager* return_val = managers_.find(typeid(ManagerType).name())->second;
		return dynamic_cast<ManagerType*>(return_val);
	}

private:
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