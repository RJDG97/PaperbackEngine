#ifndef _CORE_ENGINE_H_
#define _CORE_ENGINE_H_

#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Systems/FrameRateController.h"

#include "Manager/IManager.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

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
		
		systems_.push_back(new SystemType);
	}

	template <typename SystemType>
	SystemType* GetSystem(std::string system_name) {
		
		for (size_t i = 0; i < systems_.size(); ++i) {
			if (systems_[i]->GetName() == system_name) {
				return dynamic_cast<SystemType*>(systems_[i]);
			}
		}
		return nullptr;
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
	using SystemIt = std::vector<ISystem*>::iterator;
	std::vector<ISystem*> systems_;

	using ManagerIt = std::unordered_map<std::string, IManager*>::iterator;
	// Tracks all the managers the system uses
	std::unordered_map<std::string, IManager*> managers_;

	////The last time the game was updated
	//unsigned LastTime;
	//Is the game running (true) or being shut down (false)?
	bool b_game_active_;
};

extern CoreEngine* CORE;

#endif