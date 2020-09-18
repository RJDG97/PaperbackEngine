#include "SoundSystem.h"
#include "Core.h"
#include "InputSystem.h"

SoundSystem::SoundSystem() : bMute{ false }, bPaused{ false } {
	// Create system
	FMOD::System_Create(&f_system);
	// Initialize system
	f_system->init(32, FMOD_INIT_NORMAL, f_system);
}

SoundSystem::~SoundSystem() {
	// Iterate through sound library and release all files
	for (auto begin = soundLibrary.begin(); begin != soundLibrary.end(); ++begin) {
		begin->second->release();
	}

	// Terminate system
	f_system->close();
	f_system->release();
}

bool SoundSystem::checkError(FMOD_RESULT fResult) {
	if (fResult == FMOD_OK) {
		return 1;
	}
	else {
		std::cout << "FMOD Error: " << fResult << std::endl;
		return 0;
	}
}

void SoundSystem::loadSound(std::string fileLocation, std::string fileID, bool loopStatus) {
	// Determine whether sound file already exists within sound library
	SoundIt it = soundLibrary.find(fileID);

	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= loopStatus ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	// If sound file does not exist within sound library, include it
	if (it == soundLibrary.end()) {
		FMOD::Sound* sound = nullptr;
		// Load the sound file
		if (checkError(f_system->createSound(fileLocation.c_str(), mode, 0, &sound))) {
			// If sound file is successfully loaded
			if (sound != nullptr) {
				// Insert sound file into sound library
				soundLibrary.insert(std::pair<std::string, FMOD::Sound*>(fileID, sound));
			}
		}
	}
}

void SoundSystem::playSound(std::string fileID) {
	// Check whether sound file exists within sound library
	SoundIt it = soundLibrary.find(fileID);

	// If sound file exists within the sound library
	if (it != soundLibrary.end()) {
		// Check whether current sound file is already playing
		ChannelIt channelIT = channelLibrary.find(fileID);
		// If sound file is currently not playing
		if (channelIT == channelLibrary.end()) {
			FMOD::Channel* channel = nullptr;
			// Load the channel with the sound file and play
			if (checkError(f_system->playSound(it->second, 0, false, &channel))) {
				// If channel is playing the sound file successfully
				if (channel != nullptr) {
					// Insert channel into channel library
					channelLibrary.insert(std::pair<std::string, FMOD::Channel*>(fileID, channel));
				}
			}
		}
	}
	else {
		std::cout << fileID.c_str() << " does not exist!" << std::endl;
		return;
	}
}

void SoundSystem::stopSound(std::string fileID, bool stopAllChannels) {
	// Stop a single channel
	if (!stopAllChannels) {
		// Check whether sound file is currently playing in a channel
		ChannelIt it = channelLibrary.find(fileID);

		// If sound file is current playing in a channel
		if (it != channelLibrary.end()) {
			// Stop the channel
			it->second->stop();
		}
	}
	// Stop all channels (End of level)
	else {
		// Iterate through all active channels
		for (auto& it : channelLibrary) {
			// Stop the channel
			it.second->stop();
		}
	}
}

void SoundSystem::muteSound() {
	// If there are no active channels, return
	if (channelLibrary.begin() == channelLibrary.end())
		return;

	// Get the status of channels (Muted / Not Muted)
	channelLibrary.begin()->second->getMute(&bMute);
	// Toggle mute status
	bMute = !bMute;

	// Assign updated status to all active channels
	for (auto& it : channelLibrary) {
		it.second->setMute(bMute);
	}
}

void SoundSystem::pauseSound() {
	// If there are no active channels, return
	if (channelLibrary.begin() == channelLibrary.end())
		return;

	// Get the status of channels (Paused / Not Paused)
	channelLibrary.begin()->second->getPaused(&bPaused);
	// Toggle pause status
	bPaused = !bPaused;

	// Assign updated status to all active channels
	for (auto& it : channelLibrary) {
		it.second->setPaused(bPaused);
	}
}

void SoundSystem::removeCompletedChannel() {
	// Iterate through all completed channels
	for (auto& channel : completedChannel) {
		std::cout << "Deleting completed channel: " << channel->first.c_str() << std::endl;
		channelLibrary.erase(channel);
	}

	// Clear and resize completed channel
	completedChannel.clear();
}

void SoundSystem::init() {
	// Load all sound files
	loadSound("Resources/SoundCache/KK_BBG.mp3", "BGM", 1);
	loadSound("Resources/SoundCache/Kachow.mp3", "Kachow");
}

void SoundSystem::update(float frametime) {

	/*!
		Check all active channel's status (Done playing?)
		- Push all completed channels into vector for deletion
	*/
	{
		// Iterate through all active channels
		for (auto channel = channelLibrary.begin(); channel != channelLibrary.end(); ++channel) {
			bool bPlaying = false;
			// Get channel's status (Playing / Completed)
			channel->second->isPlaying(&bPlaying);
			if (!bPlaying) {
				// Push back the completed channel after its completed
				completedChannel.push_back(channel);
			}
		}

		if (completedChannel.size() > 0) {
			// If channel has completed playing sound file, notify
			// system to remove it
			Message msg(MessageIDTypes::BGM_Completed);
			CORE->BroadcastMessage(&msg);
		}
	}
}

std::string SoundSystem::GetName() {
	
	return { "SoundSystem" };
}

//receives message as base class pointer and uses them based on message id value
void SoundSystem::SendMessageD(Message* m) {
	
	std::cout << "Message received by SoundSystem" << std::endl;
	//plays a fileID as included in the message
	if (m->MessageID == MessageIDTypes::BGM_Play) {
		std::cout << "Message to play" << std::endl;
		MessageBGM_Play* msg = dynamic_cast<MessageBGM_Play*>(m);
		playSound(msg->_fileID);
	}

	//pauses the current BGM
	if (m->MessageID == MessageIDTypes::BGM_Pause) {
		pauseSound();
	}

	//mutes the current BGM
	if (m->MessageID == MessageIDTypes::BGM_Mute) {
		muteSound();
	}

	// stops the current BGM
	if (m->MessageID == MessageIDTypes::BGM_Stop) {
		stopSound("BGM");
	}

	// Reload bgm and play
	if (m->MessageID == MessageIDTypes::BGM_Reload) {
		loadSound("Resources/SoundCache/KK_BBG.mp3", "BGM");
		playSound("BGM");
	}

	if (m->MessageID == MessageIDTypes::BGM_Completed) {
		removeCompletedChannel();
	}
}