#ifndef _CORE_ENGINE_H_
#define _CORE_ENGINE_H_

#include "Systems/ISystem.h"
#include "Systems/Message.h"
#include "Systems/FrameRateController.h"
#include <vector>

class CoreEngine
{
public:
	CoreEngine();
	~CoreEngine() = default;
	///Update all the systems until the game is no longer active.
	void GameLoop();
	///Destroy all systems in reverse order that they were added.
	void DestroySystems();
	///Broadcasts a message to all systems.
	void BroadcastMessage(Message* m);
	///Adds a new system to the game.
	void AddSystem(ISystem* system);
	///Initializes all systems in the game.
	void Initialize();

	template <typename SystemType>
	SystemType* GetSystem(std::string system_name) {
		for (size_t i = 0; i < systems_.size(); ++i) {
			if (systems_[i]->GetName() == system_name) {
				return dynamic_cast<SystemType*>(systems_[i]);
			}
		}
		return nullptr;
	}

private:
	//Tracks all the systems the game uses
	std::vector<ISystem*> systems_;
	////The last time the game was updated
	//unsigned LastTime;
	//Is the game running (true) or being shut down (false)?
	bool b_game_active_;
};

extern CoreEngine* CORE;

#endif